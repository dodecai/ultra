module;

#include "OpenGL.h"
#include "SDL.h"

#include "ArrayList.h"
#include "Bytes.h"
#include "File.h"
#include "Metric.h"
#include "Profiler.h"
#include "RefCounted.h"

#include "Vec2.h"
#include "Vec4.h"

#include "PhxString.h"
#include "StrMap.h"

#pragma warning(push)
#pragma warning(disable:4242)
#pragma warning(disable:4244)
//https://github.com/nothings/stb/issues/334
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_STATIC
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#pragma warning(pop)

module Ultra.Engine.Phoenix;

///
/// @brief Resource
///
namespace Ultra {

struct PathElem {
    cstr format;
    PathElem *next;
};

static PathElem *paths[(int)ResourceType::Count] = { 0 };

inline static cstr Resource_Resolve(ResourceType type, cstr name, bool failhard) {
    static char buffer[256];
    for (PathElem *elem = paths[(int)type]; elem; elem = elem->next) {
        int res = snprintf(buffer, sizeof(buffer), elem->format, name);
        if (res > 0 && res < (int)sizeof(buffer)) {
            if (File_Exists(buffer))
                return buffer;
        }
    }

    /* Attempt to fall back to literal path if no path rule worked. */
    if (name && File_Exists(name))
        return name;

    if (failhard)
        Fatal("Resource_Resolve: Failed to find %s <%s>",
          Resource::ToString(type), name);

    return 0;
}

void Resource::AddPath(ResourceType type, cstr format) {
    PathElem *self = MemNew(PathElem);
    self->format = StrDup(format);
    self->next = paths[(int)type];
    paths[(int)type] = self;
}

bool Resource::Exists(ResourceType type, cstr name) {
    return Resource_Resolve(type, name, false) != 0;
}

cstr Resource::GetPath(ResourceType type, cstr name) {
    return Resource_Resolve(type, name, true);
}

Bytes *Resource::LoadBytes(ResourceType type, cstr name) {
    cstr path = Resource_Resolve(type, name, true);
    Bytes *data = File_ReadBytes(path);
    if (!data)
        Fatal("Resource_LoadBytes: Failed to load %s <%s> at <%s>",
          Resource::ToString(type), name, path);
    return data;
}

cstr Resource::LoadCstr(ResourceType type, cstr name) {
    cstr path = Resource_Resolve(type, name, true);
    cstr data = File_ReadCstr(path);
    if (!data)
        Fatal("Resource_LoadCstr: Failed to load %s <%s> at <%s>",
          Resource::ToString(type), name, path);
    return data;
}

cstr Resource::ToString(ResourceType type) {
    switch (type) {
        case ResourceType::Font:    return "Font";
        case ResourceType::Mesh:    return "Mesh";
        case ResourceType::Other:   return "Other";
        case ResourceType::Script:  return "Script";
        case ResourceType::Shader:  return "Shader";
        case ResourceType::Sound:   return "Sound";
        case ResourceType::Tex1D:   return "Tex1D";
        case ResourceType::Tex2D:   return "Tex2D";
        case ResourceType::Tex3D:   return "Tex3D";
        case ResourceType::TexCube: return "TexCube";
    }
    return 0;
}

void Resource::Init() {
    Resource::AddPath(ResourceType::Font, "%s.ttf");
    Resource::AddPath(ResourceType::Font, "%s.otf");
    Resource::AddPath(ResourceType::Mesh, "%s.bin");
    Resource::AddPath(ResourceType::Mesh, "%s.obj");
    Resource::AddPath(ResourceType::Other, "%s");
    Resource::AddPath(ResourceType::Script, "%s.lua");
    Resource::AddPath(ResourceType::Shader, "%s.glsl");
    Resource::AddPath(ResourceType::Sound, "%s.mp3");
    Resource::AddPath(ResourceType::Sound, "%s.ogg");
    Resource::AddPath(ResourceType::Sound, "%s.ogx");
    Resource::AddPath(ResourceType::Sound, "%s.wav");
    Resource::AddPath(ResourceType::Tex1D, "%s.bin");
    Resource::AddPath(ResourceType::Tex2D, "%s.jpg");
    Resource::AddPath(ResourceType::Tex2D, "%s.png");
    Resource::AddPath(ResourceType::Tex3D, "%s.bin");
    Resource::AddPath(ResourceType::TexCube, "%s");

    Resource::AddPath(ResourceType::Font, "./Assets/Phoenix/res/font/%s.ttf");
    Resource::AddPath(ResourceType::Font, "./Assets/Phoenix/res/font/%s.otf");
    Resource::AddPath(ResourceType::Mesh, "./Assets/Phoenix/res/mesh/%s.bin");
    Resource::AddPath(ResourceType::Mesh, "./Assets/Phoenix/res/mesh/%s.obj");
    Resource::AddPath(ResourceType::Other, "./Assets/Phoenix/res/%s");
    Resource::AddPath(ResourceType::Script, "./Assets/Phoenix/res/script/%s.lua");
    Resource::AddPath(ResourceType::Shader, "./Assets/Phoenix/res/shader/%s.glsl");
    Resource::AddPath(ResourceType::Sound, "./Assets/Phoenix/res/sound/%s.mp3");
    Resource::AddPath(ResourceType::Sound, "./Assets/Phoenix/res/sound/%s.ogg");
    Resource::AddPath(ResourceType::Sound, "./Assets/Phoenix/res/sound/%s.ogx");
    Resource::AddPath(ResourceType::Sound, "./Assets/Phoenix/res/sound/%s.wav");
    Resource::AddPath(ResourceType::Tex1D, "./Assets/Phoenix/res/tex1d/%s.bin");
    Resource::AddPath(ResourceType::Tex2D, "./Assets/Phoenix/res/tex2d/%s.jpg");
    Resource::AddPath(ResourceType::Tex2D, "./Assets/Phoenix/res/tex2d/%s.png");
    Resource::AddPath(ResourceType::Tex3D, "./Assets/Phoenix/res/tex3d/%s.bin");
    Resource::AddPath(ResourceType::TexCube, "./Assets/Phoenix/res/texcube/%s");
}

}

///
/// @brief Shader
///
namespace Ultra {

cstr ShaderVarTypeGetGLSLName(ShaderVarType self) {
    switch (self) {
        case ShaderVarType::Float:   return "float";
        case ShaderVarType::Float2:  return "vec2";
        case ShaderVarType::Float3:  return "vec3";
        case ShaderVarType::Float4:  return "vec4";
        case ShaderVarType::Int:     return "int";
        case ShaderVarType::Int2:    return "ivec2";
        case ShaderVarType::Int3:    return "ivec3";
        case ShaderVarType::Int4:    return "ivec4";
        case ShaderVarType::Matrix:  return "mat4";
        case ShaderVarType::Tex1D:   return "sampler1D";
        case ShaderVarType::Tex2D:   return "sampler2D";
        case ShaderVarType::Tex3D:   return "sampler3D";
        case ShaderVarType::TexCube: return "samplerCube";
    }
    return 0;
}

ShaderVarType ShaderVarTypeFromStr(cstr s) {
    for (int i = (int)ShaderVarType::BEGIN; i <= (int)ShaderVarType::END; ++i)
        if (StrEqual(s, ShaderVarTypeGetGLSLName((ShaderVarType)i))) return (ShaderVarType)i;
    return ShaderVarType::None;
}

cstr ShaderVarTypeGetName(ShaderVarType self) {
    switch (self) {
        case ShaderVarType::Float:   return "float";
        case ShaderVarType::Float2:  return "float2";
        case ShaderVarType::Float3:  return "float3";
        case ShaderVarType::Float4:  return "float4";
        case ShaderVarType::Int:     return "int";
        case ShaderVarType::Int2:    return "int2";
        case ShaderVarType::Int3:    return "int3";
        case ShaderVarType::Int4:    return "int4";
        case ShaderVarType::Matrix:  return "Matrix";
        case ShaderVarType::Tex1D:   return "Tex1D";
        case ShaderVarType::Tex2D:   return "Tex2D";
        case ShaderVarType::Tex3D:   return "Tex3D";
        case ShaderVarType::TexCube: return "TexCube";
    }
    return 0;
}

int ShaderVarTypeGetSize(ShaderVarType self) {
    switch (self) {
        case ShaderVarType::Float:   return sizeof(float);
        case ShaderVarType::Float2:  return sizeof(Vec2f);
        case ShaderVarType::Float3:  return sizeof(Vec3f);
        case ShaderVarType::Float4:  return sizeof(Vec4f);
        case ShaderVarType::Int:     return sizeof(int);
        case ShaderVarType::Int2:    return sizeof(Vec2i);
        case ShaderVarType::Int3:    return sizeof(Vec3i);
        case ShaderVarType::Int4:    return sizeof(Vec4i);
        case ShaderVarType::Matrix:  return sizeof(Matrix *);
        case ShaderVarType::Tex1D:   return sizeof(Tex1D *);
        case ShaderVarType::Tex2D:   return sizeof(Tex2D *);
        case ShaderVarType::Tex3D:   return sizeof(Tex3D *);
        case ShaderVarType::TexCube: return sizeof(TexCube *);
    }
    return 0;
}


/* TODO : Implement custom directives to mimic layout functionality from GL3+. */
/* TODO : Use glShaderSource's array functionality to implement include files
 *        elegantly. */

static cstr includePath = "include/";
static cstr versionString = "#version 130\n";

struct ShaderVar {
    ShaderVarType type;
    cstr name;
    int index;
};

struct ShaderData {
    RefCounted;
    cstr name;
    uint vs;
    uint fs;
    uint program;
    uint texIndex;
    ArrayList(ShaderVar, vars);
};

static ShaderData *current = 0;
static unordered_map<string, string> cache;

static cstr GLSL_Load(cstr path, ShaderData *);
static cstr GLSL_Preprocess(cstr code, ShaderData *);

static int GetUniformIndex(ShaderData *self, cstr name, bool mustSucceed = false) {
    if (!self)
        Fatal("GetUniformIndex: No shader is bound");
    int index = glGetUniformLocation(self->program, name);
    if (index == -1 && mustSucceed)
        Fatal("GetUniformIndex: Shader <%s> has no variable <%s>", self->name, name);
    return index;
}

static uint CreateGLShader(cstr src, GLenum type) {
    uint self = glCreateShader(type);

    cstr srcs[] = {
      versionString,
      src,
    };

    GLCALL(glShaderSource(self, 2, srcs, 0))
        GLCALL(glCompileShader(self))

        /* Check for compile errors. */ {
        int status;
        GLCALL(glGetShaderiv(self, GL_COMPILE_STATUS, &status))
            if (status == GL_FALSE) {
                int length;
                GLCALL(glGetShaderiv(self, GL_INFO_LOG_LENGTH, &length))
                    char *infoLog = (char *)MemAllocZero(length + 1);
                GLCALL(glGetShaderInfoLog(self, length, 0, infoLog))
                    Fatal("CreateGLShader: Failed to compile shader:\n%s", infoLog);
            }
    }
    return self;
}

static uint CreateGLProgram(uint vs, uint fs) {
    uint self = glCreateProgram();
    GLCALL(glAttachShader(self, vs))
        GLCALL(glAttachShader(self, fs))

        /* TODO : Replace with custom directives. */ {
        GLCALL(glBindAttribLocation(self, 0, "vertex_position"))
            GLCALL(glBindAttribLocation(self, 1, "vertex_normal"))
            GLCALL(glBindAttribLocation(self, 2, "vertex_uv"))
    }

    GLCALL(glLinkProgram(self))

        /* Check for link errors. */ {
        int status;
        GLCALL(glGetProgramiv(self, GL_LINK_STATUS, &status))
            if (status == GL_FALSE) {
                int length;
                GLCALL(glGetProgramiv(self, GL_INFO_LOG_LENGTH, &length))
                    char *infoLog = (char *)MemAllocZero(length + 1);
                GLCALL(glGetProgramInfoLog(self, length, 0, infoLog))
                    Fatal("CreateGLProgram: Failed to link program:\n%s", infoLog);
            }
    }
    return self;
}

/* BUG : Cache does not contain information about custom preprocessor
 *       directives, hence cached shaders with custom directives do not work */
static cstr GLSL_Load(cstr name, ShaderData *self) {
    if (cache.empty()) cache.reserve(16);
    auto cached = cache[name];
    if (!cached.empty()) return cached.c_str();
    cstr rawCode = Resource::LoadCstr(ResourceType::Shader, name);
    cstr code = StrReplace(rawCode, "\r\n", "\n");
    StrFree(rawCode);
    code = GLSL_Preprocess(code, self);
    /* BUG : Disable GLSL caching until preprocessor cache works. */
    // StrMap_Set(cache, name, (void*)code);
    return code;
}

static cstr GLSL_Preprocess(cstr code, ShaderData *self) {
    const int lenInclude = StrLen("#include");
    cstr begin;

    /* Parse Includes. */
    while ((begin = StrFind(code, "#include")) != 0) {
        cstr end = StrFind(begin, "\n");
        cstr name = StrSubStr(begin + lenInclude + 1, end);
        cstr path = StrAdd(includePath, name);
        cstr prev = code;
        code = StrSub(code, begin, end, GLSL_Load(path, self));
        StrFree(prev);
        StrFree(path);
        StrFree(name);
    }

    /* Parse automatic ShaderVar stack bindings. */
    while ((begin = StrFind(code, "#autovar")) != 0) {
        cstr end = StrFind(begin, "\n");
        cstr line = StrSubStr(begin, end);
        char varType[32] = { 0 };
        char varName[32] = { 0 };

        if (sscanf(line, "#autovar %31s %31s", varType, varName) == 2) {
            ShaderVar var = {};
            var.type = ShaderVarTypeFromStr(varType);
            if (var.type == ShaderVarType::None)
                Fatal("GLSL_Preprocess: Unknown shader variable type <%s> "
                      "in directive:\n  %s", varType, line);
            var.name = StrDup(varName);
            var.index = -1;
            ArrayList_Append(self->vars, var);
        } else {
            Fatal("GLSL_Preprocess: Failed to parse directive:\n  %s", line);
        }

        cstr prev = code;
        code = StrSub(code, begin, end, "");
        StrFree(prev);
        StrFree(line);
    }
    return code;
}

static void Shader_BindVariables(ShaderData *self) {
    for (int i = 0; i < ArrayList_GetSize(self->vars); ++i) {
        ShaderVar *var = ArrayList_GetPtr(self->vars, i);
        var->index = glGetUniformLocation(self->program, var->name);
        if (var->index < 0)
            Warn("Shader_BindVariables: Automatic shader variable <%s> does not exist"
                 " in shader <%s>", var->name, self->name);
    }
}

/* --- Creation ------------------------------------------------------------- */

ShaderData *PhxShader::Create(cstr vs, cstr fs) {
    ShaderData *self = MemNew(ShaderData);
    RefCounted_Init(self);
    ArrayList_Init(self->vars);
    vs = GLSL_Preprocess(StrDup(vs), self);
    fs = GLSL_Preprocess(StrDup(fs), self);
    self->vs = CreateGLShader(vs, GL_VERTEX_SHADER);
    self->fs = CreateGLShader(fs, GL_FRAGMENT_SHADER);
    self->program = CreateGLProgram(self->vs, self->fs);
    self->texIndex = 1;
    self->name = StrFormat("[anonymous shader @ %p]", self);
    StrFree(vs);
    StrFree(fs);
    Shader_BindVariables(self);
    return self;
}

ShaderData *PhxShader::Load(cstr vName, cstr fName) {
    ShaderData *self = MemNew(ShaderData);
    RefCounted_Init(self);
    ArrayList_Init(self->vars);
    cstr vs = GLSL_Load(vName, self);
    cstr fs = GLSL_Load(fName, self);
    self->vs = CreateGLShader(vs, GL_VERTEX_SHADER);
    self->fs = CreateGLShader(fs, GL_FRAGMENT_SHADER);
    self->program = CreateGLProgram(self->vs, self->fs);
    self->texIndex = 1;
    self->name = StrFormat("[vs: %s , fs: %s]", vName, fName);
    Shader_BindVariables(self);
    return self;
}

void PhxShader::Acquire(ShaderData *self) {
    RefCounted_Acquire(self);
}

void PhxShader::Free(ShaderData *self) {
    RefCounted_Free(self) {
        GLCALL(glDeleteShader(self->vs))
            GLCALL(glDeleteShader(self->fs))
            GLCALL(glDeleteProgram(self->program))
            ArrayList_Free(self->vars);
        StrFree(self->name);
        MemFree(self);
    }
}

ShaderState *PhxShader::ToShaderState(ShaderData *self) {
    // ToDo: Migrate Shader State
    //return ShaderState_Create((::Shader*)self);
    return {};
}

/* --- Usage ---------------------------------------------------------------- */

void PhxShader::Start(ShaderData *self) {
    FRAME_BEGIN;
    GLCALL(glUseProgram(self->program))
        current = self;
    self->texIndex = 1;

    /* Fetch & bind automatic variables from the shader var stack. */
    for (int i = 0; i < ArrayList_GetSize(self->vars); ++i) {
        ShaderVar *var = ArrayList_GetPtr(self->vars, i);
        if (var->index < 0) continue;
        void *pValue = PhxShaderVar::Get(var->name, var->type);
        if (!pValue)
            Fatal("Shader_Start: Shader variable stack does not contain variable <%s>", var->name);

        switch (var->type) {
            case ShaderVarType::Float: {
                float value = *(float *)pValue;
                GLCALL(glUniform1f(var->index, value));
                break; }
            case ShaderVarType::Float2: {
                Vec2f value = *(Vec2f *)pValue;
                GLCALL(glUniform2f(var->index, value.x, value.y));
                break; }
            case ShaderVarType::Float3: {
                Vec3f value = *(Vec3f *)pValue;
                GLCALL(glUniform3f(var->index, value.x, value.y, value.z));
                break; }
            case ShaderVarType::Float4: {
                Vec4f value = *(Vec4f *)pValue;
                GLCALL(glUniform4f(var->index, value.x, value.y, value.z, value.w));
                break; }
            case ShaderVarType::Int: {
                int value = *(int *)pValue;
                GLCALL(glUniform1i(var->index, value));
                break; }
            case ShaderVarType::Int2: {
                Vec2i value = *(Vec2i *)pValue;
                GLCALL(glUniform2i(var->index, value.x, value.y));
                break; }
            case ShaderVarType::Int3: {
                Vec3i value = *(Vec3i *)pValue;
                GLCALL(glUniform3i(var->index, value.x, value.y, value.z));
                break; }
            case ShaderVarType::Int4: {
                Vec4i value = *(Vec4i *)pValue;
                GLCALL(glUniform4i(var->index, value.x, value.y, value.z, value.w));
                break; }
            case ShaderVarType::Matrix: {
                ISetMatrix(var->index, *(Matrix **)pValue);
                break; }
            case ShaderVarType::Tex1D: {
                //ISetTex1D(var->index, *(Tex1D **)pValue);
                break; }
            case ShaderVarType::Tex2D: {
                ISetTex2D(var->index, *(Tex2DData **)pValue);
                break; }
            case ShaderVarType::Tex3D: {
                //ISetTex3D(var->index, *(Tex3D **)pValue);
                break; }
            case ShaderVarType::TexCube: {
                //ISetTexCube(var->index, *(TexCube **)pValue);
                break; }
        }
    }
    FRAME_END;
}

void PhxShader::Stop(ShaderData *) {
    GLCALL(glUseProgram(0))
        current = 0;
}

static void ShaderCache_FreeElem(cstr, void *data) {
    free(data);
}

void PhxShader::ClearCache() {
    if (!cache.empty()) {
        cache.clear();
    }
}

uint PhxShader::GetHandle(ShaderData *self) {
    return self->program;
}

int PhxShader::GetVariable(ShaderData *self, cstr name) {
    int index = glGetUniformLocation(self->program, name);
    if (index == -1)
        Fatal("Shader_GetVariable: Shader <%s> has no variable <%s>", self->name, name);
    return index;
}

bool PhxShader::HasVariable(ShaderData *self, cstr name) {
    return glGetUniformLocation(self->program, name) > -1;
}

/* --- Variable Binding ----------------------------------------------------- */

void PhxShader::SetFloat(cstr name, float value) {
    GLCALL(glUniform1f(GetUniformIndex(current, name), value))
}

void PhxShader::ISetFloat(int index, float value) {
    GLCALL(glUniform1f(index, value))
}

void PhxShader::SetFloat2(cstr name, float x, float y) {
    GLCALL(glUniform2f(GetUniformIndex(current, name), x, y))
}

void PhxShader::ISetFloat2(int index, float x, float y) {
    GLCALL(glUniform2f(index, x, y))
}

void PhxShader::SetFloat3(cstr name, float x, float y, float z) {
    GLCALL(glUniform3f(GetUniformIndex(current, name), x, y, z))
}

void PhxShader::ISetFloat3(int index, float x, float y, float z) {
    GLCALL(glUniform3f(index, x, y, z))
}

void PhxShader::SetFloat4(cstr name, float x, float y, float z, float w) {
    GLCALL(glUniform4f(GetUniformIndex(current, name), x, y, z, w))
}

void PhxShader::ISetFloat4(int index, float x, float y, float z, float w) {
    GLCALL(glUniform4f(index, x, y, z, w))
}

void PhxShader::SetInt(cstr name, int value) {
    GLCALL(glUniform1i(GetUniformIndex(current, name), value))
}

void PhxShader::ISetInt(int index, int value) {
    GLCALL(glUniform1i(index, value))
}

void PhxShader::SetMatrix(cstr name, Matrix *value) {
    GLCALL(glUniformMatrix4fv(GetUniformIndex(current, name), 1, true, (float *)value))
}

void PhxShader::SetMatrixT(cstr name, Matrix *value) {
    GLCALL(glUniformMatrix4fv(GetUniformIndex(current, name), 1, false, (float *)value))
}

void PhxShader::ISetMatrix(int index, Matrix *value) {
    GLCALL(glUniformMatrix4fv(index, 1, true, (float *)value))
}

void PhxShader::ISetMatrixT(int index, Matrix *value) {
    GLCALL(glUniformMatrix4fv(index, 1, false, (float *)value))
}

//void PhxShader::SetTex1D(cstr name, Tex1D *value) {
//    GLCALL(glUniform1i(GetUniformIndex(current, name), current->texIndex))
//        GLCALL(glActiveTexture(GL_TEXTURE0 + current->texIndex++))
//        GLCALL(glBindTexture(GL_TEXTURE_1D, Tex1D_GetHandle(value)))
//        GLCALL(glActiveTexture(GL_TEXTURE0))
//}
//
//void PhxShader::ISetTex1D(int index, Tex1D *value) {
//    GLCALL(glUniform1i(index, current->texIndex))
//        GLCALL(glActiveTexture(GL_TEXTURE0 + current->texIndex++))
//        GLCALL(glBindTexture(GL_TEXTURE_1D, Tex1D_GetHandle(value)))
//        GLCALL(glActiveTexture(GL_TEXTURE0))
//}

void PhxShader::SetTex2D(cstr name, Tex2DData *value) {
    GLCALL(glUniform1i(GetUniformIndex(current, name), current->texIndex))
        GLCALL(glActiveTexture(GL_TEXTURE0 + current->texIndex++))
        GLCALL(glBindTexture(GL_TEXTURE_2D, Tex2D::GetHandle(value)))
        GLCALL(glActiveTexture(GL_TEXTURE0))
}

void PhxShader::ISetTex2D(int index, Tex2DData *value) {
    GLCALL(glUniform1i(index, current->texIndex))
        GLCALL(glActiveTexture(GL_TEXTURE0 + current->texIndex++))
        GLCALL(glBindTexture(GL_TEXTURE_2D, Tex2D::GetHandle(value)))
        GLCALL(glActiveTexture(GL_TEXTURE0))
}

//void PhxShader::SetTex3D(cstr name, Tex3D *value) {
//    GLCALL(glUniform1i(GetUniformIndex(current, name), current->texIndex))
//        GLCALL(glActiveTexture(GL_TEXTURE0 + current->texIndex++))
//        GLCALL(glBindTexture(GL_TEXTURE_3D, Tex3D_GetHandle(value)))
//        GLCALL(glActiveTexture(GL_TEXTURE0))
//}
//
//void PhxShader::ISetTex3D(int index, Tex3D *value) {
//    GLCALL(glUniform1i(index, current->texIndex))
//        GLCALL(glActiveTexture(GL_TEXTURE0 + current->texIndex++))
//        GLCALL(glBindTexture(GL_TEXTURE_3D, Tex3D_GetHandle(value)))
//        GLCALL(glActiveTexture(GL_TEXTURE0))
//}
//
//void PhxShader::SetTexCube(cstr name, TexCube *value) {
//    GLCALL(glUniform1i(GetUniformIndex(current, name), current->texIndex))
//        GLCALL(glActiveTexture(GL_TEXTURE0 + current->texIndex++))
//        GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, TexCube_GetHandle(value)))
//        GLCALL(glActiveTexture(GL_TEXTURE0))
//}
//
//void PhxShader::ISetTexCube(int index, TexCube *value) {
//    GLCALL(glUniform1i(index, current->texIndex))
//        GLCALL(glActiveTexture(GL_TEXTURE0 + current->texIndex++))
//        GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, TexCube_GetHandle(value)))
//        GLCALL(glActiveTexture(GL_TEXTURE0))
//}



#define DEFAULT_CAPACITY 4

struct VarStack {
    ShaderVarType type;
    int32 size;
    int32 capacity;
    int32 elemSize;
    void *data;
};

static StrMap *varMap = 0;

inline static VarStack *ShaderVar_GetStack(cstr var, ShaderVarType type) {
    VarStack *self = (VarStack *)StrMap_Get(varMap, var);
    if (!self) {
        //if (type) return 0;
        self = MemNew(VarStack);
        self->type = type;
        self->size = 0;
        self->capacity = DEFAULT_CAPACITY;
        self->elemSize = ShaderVarTypeGetSize(type);
        self->data = MemAlloc(self->capacity * self->elemSize);
        StrMap_Set(varMap, var, self);
    }

    if ((int)type && (int)self->type != (int)type)
        Fatal("ShaderVar_GetStack: Attempting to get stack of type <%s>"
              " for shader variable <%s> when existing stack has type <%s>",
              ShaderVarTypeGetName(type), var, ShaderVarTypeGetName(self->type));

    return self;
}

inline static void ShaderVar_Push(cstr var, ShaderVarType type, void const *value) {
    VarStack *self = ShaderVar_GetStack(var, type);
    if (self->size == self->capacity) {
        self->capacity *= 2;
        self->data = MemRealloc(self->data, self->capacity * self->elemSize);
    }
    MemCpy((char *)self->data + self->size * self->elemSize, value, self->elemSize);
    self->size++;
}

void PhxShaderVar::Init() {
    varMap = StrMap_Create(16);
}

void PhxShaderVar::Free() {
    StrMap_Free(varMap);
    varMap = 0;
}

void *PhxShaderVar::Get(cstr name, ShaderVarType type) {
    VarStack *self = ShaderVar_GetStack(name, ShaderVarType::None);
    if (!self || self->size == 0)
        return 0;
    if ((int)type && (int)self->type != (int)type)
        Fatal("ShaderVar_Get: Attempting to get variable <%s> with type <%s> when"
              " existing stack has type <%s>",
              name, ShaderVarTypeGetName(type), ShaderVarTypeGetName(self->type));
    return (char *)self->data + self->elemSize * (self->size - 1);
}

void PhxShaderVar::PushFloat(cstr name, float x) {
    ShaderVar_Push(name, ShaderVarType::Float, &x);
}

void PhxShaderVar::PushFloat2(cstr name, float x, float y) {
    Vec2f value = { x, y };
    ShaderVar_Push(name, ShaderVarType::Float2, &value);
}

void PhxShaderVar::PushFloat3(cstr name, float x, float y, float z) {
    Vec3f value = { x, y, z };
    ShaderVar_Push(name, ShaderVarType::Float3, &value);
}

void PhxShaderVar::PushFloat4(cstr name, float x, float y, float z, float w) {
    Vec4f value = { x, y, z, w };
    ShaderVar_Push(name, ShaderVarType::Float4, &value);
}

void PhxShaderVar::PushInt(cstr name, int x) {
    int32 value = (int32)x;
    ShaderVar_Push(name, ShaderVarType::Int, &value);
}

void PhxShaderVar::PushMatrix(cstr name, Matrix *x) {
    ShaderVar_Push(name, ShaderVarType::Matrix, &x);
}

void PhxShaderVar::PushTex1D(cstr name, Tex1D *x) {
    ShaderVar_Push(name, ShaderVarType::Tex1D, &x);
}

void PhxShaderVar::PushTex2D(cstr name, Tex2DData *x) {
    ShaderVar_Push(name, ShaderVarType::Tex2D, &x);
}

void PhxShaderVar::PushTex3D(cstr name, Tex3D *x) {
    ShaderVar_Push(name, ShaderVarType::Tex3D, &x);
}

void PhxShaderVar::PushTexCube(cstr name, TexCube *x) {
    ShaderVar_Push(name, ShaderVarType::TexCube, &x);
}

void PhxShaderVar::Pop(cstr name) {
    VarStack *self = ShaderVar_GetStack(name, ShaderVarType::None);
    if (!self)
        Fatal("ShaderVar_Pop: Attempting to pop nonexistent stack <%s>", name);
    if (self->size == 0)
        Fatal("ShaderVar_Pop: Attempting to pop empty stack <%s>", name);
    self->size--;
}

}

///
/// @brief Texture2D
///
namespace Ultra {

GLint GetGLDataFormat(PhxDataFormat format) {
    switch (format) {
        case PhxDataFormat::U8: return GL_UNSIGNED_BYTE;
        case PhxDataFormat::I8: return GL_BYTE;
        case PhxDataFormat::U16: return GL_UNSIGNED_SHORT;
        case PhxDataFormat::I16: return GL_SHORT;
        case PhxDataFormat::U32: return GL_UNSIGNED_INT;
        case PhxDataFormat::I32: return GL_INT;
        case PhxDataFormat::Float: return GL_FLOAT;
    }
}

GLint GetGLPixelFormat(PhxPixelFormat format) {
    switch (format) {
        case PhxPixelFormat::Red: return GL_RED;
        case PhxPixelFormat::RG: return GL_RG;
        case PhxPixelFormat::RGB: return GL_RGB;
        case PhxPixelFormat::BGR: return GL_BGR;
        case PhxPixelFormat::RGBA: return GL_RGBA;
        case PhxPixelFormat::BGRA: return GL_BGRA;
        case PhxPixelFormat::DepthComponent: return GL_DEPTH_COMPONENT;
    }
}

GLint GetGLTextureFormat(PhxTextureFormat format) {
    switch (format) {
        case PhxTextureFormat::R8: return GL_R8;
        case PhxTextureFormat::R16 : return GL_R16;
        case PhxTextureFormat::R16F : return GL_R16F;
        case PhxTextureFormat::R32F : return GL_R32F;
        case PhxTextureFormat::RG8 : return GL_RG8;
        case PhxTextureFormat::RG16 : return GL_RG16;
        case PhxTextureFormat::RG16F : return GL_RG16F;
        case PhxTextureFormat::RG32F : return GL_RG32F;
        case PhxTextureFormat::RGB8 : return GL_RGB8;
        case PhxTextureFormat::RGBA8 : return GL_RGBA8;
        case PhxTextureFormat::RGBA16 : return GL_RGBA16;
        case PhxTextureFormat::RGBA16F : return GL_RGBA16F;
        case PhxTextureFormat::RGBA32F : return GL_RGBA32F;
        case PhxTextureFormat::Depth16 : return GL_DEPTH_COMPONENT16;
        case PhxTextureFormat::Depth24 : return GL_DEPTH_COMPONENT24;
        case PhxTextureFormat::Depth32F : return GL_DEPTH_COMPONENT32F;
    }
}



struct Tex2DData {
    RefCounted;
    uint handle;
    Vec2i size;
    PhxTextureFormat format;
};

inline static void Tex2D_Init() {
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST))
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST))
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
}

static uchar *Tex2D_LoadRaw(cstr path, int *sx, int *sy, int *components) {
    uchar *data = stbi_load(path, sx, sy, components, 0);
    if (!data)
        Fatal("Failed to load image from '%s'", path);
    return data;
}

static bool Tex2D_Save_Png(cstr path, int sx, int sy, int components, uchar *data) {
    int stride = components * sx;
    /*int result = stbi_write_png(path, sx, sy, components, data, stride);*/
    int result = 0;
    return result != 0;
}



Tex2DData *Tex2D::Create(int sx, int sy, PhxTextureFormat format) {
    if (!IsPhxTextureFormatValid(format))
        Fatal("Tex2D_Create: Invalid texture format requested");

    Tex2DData *self = new Tex2DData();
    RefCounted_Init(self);

    self->size = Vec2i_Create(sx, sy);
    self->format = format;

    GLCALL(glGenTextures(1, &self->handle))
        GLCALL(glActiveTexture(GL_TEXTURE0))
        GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0,
                            GetGLTextureFormat(self->format), self->size.x, self->size.y, 0,
                            IsPhxTextureFormatColor(format) ? GL_RED : GL_DEPTH_COMPONENT,
                            GL_UNSIGNED_BYTE, 0))
        Tex2D_Init();
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
        return self;
}

Tex2DData *Tex2D::ScreenCapture() {
    Vec2i size; Viewport::GetSize(&size);
    Tex2DData *self = Create(size.x, size.y, PhxTextureFormat::RGBA8);
    vector<uint32_t> buffer(size.x * size.y);
    Metric_Inc(Metric_Flush);
    GLCALL(glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data()))
        for (int y = 0; y < size.y / 2; ++y)
            for (int x = 0; x < size.x; ++x)
                Swap(buffer[size.x * y + x], buffer[size.x * (size.y - y - 1) + x]);

    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GetGLTextureFormat(PhxTextureFormat::RGBA8), size.x, size.y,
                            0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data()))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
        return self;
}

void Tex2D::Acquire(Tex2DData *self) {
    RefCounted_Acquire(self);
}

void Tex2D::Free(Tex2DData *self) {
    RefCounted_Free(self) {
        GLCALL(glDeleteTextures(1, &self->handle))
            delete self;
    }
}

void Tex2D::Pop(Tex2DData *) {
    RenderTarget::Pop();
}

void Tex2D::Push(Tex2DData *self) {
    RenderTarget::PushTex2D(self);
}

void Tex2D::PushLevel(Tex2DData *self, int level) {
    RenderTarget::PushTex2DLevel(self, level);
}

void Tex2D::Clear(Tex2DData *self, float r, float g, float b, float a) {
    RenderTarget::PushTex2D(self);
    Draw::Clear(r, g, b, a);
    RenderTarget::Pop();
}

Tex2DData *Tex2D::Clone(Tex2DData *self) {
    Tex2DData *clone = Create(self->size.x, self->size.y, self->format);
    RenderTarget::PushTex2D(self);
    GLCALL(glBindTexture(GL_TEXTURE_2D, clone->handle))
        GLCALL(glCopyTexImage2D(GL_TEXTURE_2D, 0, GetGLTextureFormat(self->format), 0, 0, self->size.x, self->size.y, 0))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
        RenderTarget::Pop();
    return clone;
}

void Tex2D::Draw(Tex2DData *self, float x, float y, float sx, float sy) {
    Metric_AddDrawImm(1, 2, 4);
    GLCALL(glEnable(GL_TEXTURE_2D))
        GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x, y);
    glTexCoord2f(0, 1); glVertex2f(x, y + sy);
    glTexCoord2f(1, 1); glVertex2f(x + sx, y + sy);
    glTexCoord2f(1, 0); glVertex2f(x + sx, y);
    GLCALL(glEnd())
        GLCALL(glDisable(GL_TEXTURE_2D))
}

void Tex2D::DrawEx(Tex2DData *self, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1) {
    Metric_AddDrawImm(1, 2, 4);
    GLCALL(glEnable(GL_TEXTURE_2D))
        GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        glBegin(GL_QUADS);
    glTexCoord2f(u0, v0); glVertex2f(x0, y0);
    glTexCoord2f(u0, v1); glVertex2f(x0, y1);
    glTexCoord2f(u1, v1); glVertex2f(x1, y1);
    glTexCoord2f(u1, v0); glVertex2f(x1, y0);
    GLCALL(glEnd())
        GLCALL(glDisable(GL_TEXTURE_2D))
}

void Tex2D::GenMipmap(Tex2DData *self) {
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glGenerateMipmap(GL_TEXTURE_2D))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
}

void Tex2D::GetData(Tex2DData *self, void *data, PhxPixelFormat pf, PhxDataFormat df) {
    Metric_Inc(Metric_Flush);
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glGetTexImage(GL_TEXTURE_2D, 0, GetGLPixelFormat(pf), GetGLDataFormat(df), data))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
}

Bytes *Tex2D::GetDataBytes(Tex2DData *self, PhxPixelFormat pf, PhxDataFormat df) {
    int size = self->size.x * self->size.y;
    size *= GetPhxDataFormatSize(df);
    size *= GetPhxPixelFormatComponents(pf);
    Bytes *data = Bytes_Create(size);
    GetData(self, Bytes_GetData(data), pf, df);
    Bytes_Rewind(data);
    return data;
}

PhxTextureFormat Tex2D::GetFormat(Tex2DData *self) {
    return self->format;
}

uint Tex2D::GetHandle(Tex2DData *self) {
    return self->handle;
}

void Tex2D::GetSize(Tex2DData *self, Vec2i *out) {
    *out = self->size;
}

void Tex2D::GetSizeLevel(Tex2DData *self, Vec2i *out, int level) {
    *out = self->size;
    for (int i = 0; i < level; ++i) {
        out->x /= 2;
        out->y /= 2;
    }
}

Tex2DData *Tex2D::Load(cstr name) {
    cstr path = Resource::GetPath(ResourceType::Tex2D, name);
    int sx, sy, components = 4;
    uchar *data = Tex2D_LoadRaw(path, &sx, &sy, &components);
    Tex2DData *self = Create(sx, sy, PhxTextureFormat::RGBA8);

    GLenum format =
        components == 4 ? GL_RGBA :
        components == 3 ? GL_RGB :
        components == 2 ? GL_RG : GL_RED;

    GLCALL(glActiveTexture(GL_TEXTURE0))
        GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0,
                            GL_RGBA8, self->size.x, self->size.y, 0,
                            format, GL_UNSIGNED_BYTE, data))
        Tex2D_Init();
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0))

        delete data;
    return self;
}

void Tex2D::SetAnisotropy(Tex2DData *self, float factor) {
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, factor))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
}

void Tex2D::SetData(Tex2DData *self, void const *data, PhxPixelFormat pf, PhxDataFormat df) {
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexImage2D(GL_TEXTURE_2D,
                            0, GetGLTextureFormat(self->format),
                            self->size.x, self->size.y,
                            0, GetGLPixelFormat(pf), GetGLDataFormat(df), data))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
}

void Tex2D::SetDataBytes(Tex2DData *self, Bytes *data, PhxPixelFormat pf, PhxDataFormat df) {
    SetData(self, Bytes_GetData(data), pf, df);
}

void Tex2D::SetMagFilter(Tex2DData *self, TexFilter filter) {
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
}

void Tex2D::SetMinFilter(Tex2DData *self, TexFilter filter) {
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
}

/* NOTE : In general, using BASE_LEVEL, MAX_LEVEL, and MIN/MAX_LOD params is
 *        dangerous due to known bugs in old Radeon & Intel drivers. See:
 *        (https://www.opengl.org/discussion_boards/showthread.php/
 *         166266-Using-GL_TEXTURE_BASE_LEVEL-with-a-comple-texture)
 *
 *        However, constraining the mip range to a single level (minLevel ==
 *        maxLevel) seems to be acceptable even on bad drivers. Thus, it is
 *        strongly advised to use this function only to constrain sampling to
 *        a single mip level. */
void Tex2D::SetMipRange(Tex2DData *self, int minLevel, int maxLevel) {
    if (minLevel != maxLevel)
        Warn("Tex2D_SetMipRange: Setting mip range with min != max; this may fail"
             " on old drivers with mip-handling bugs.");
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, minLevel))
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel))
}

void Tex2D::SetTexel(Tex2DData *self, int x, int y, float r, float g, float b, float a) {
    float rgba[] = { r, g, b, a };
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, rgba))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
}

void Tex2D::SetWrapMode(Tex2DData *self, TexWrapMode mode) {
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode))
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode))
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
}

void Tex2D::Save(Tex2DData *self, cstr path) {
    Metric_Inc(Metric_Flush);
    GLCALL(glBindTexture(GL_TEXTURE_2D, self->handle))
        vector<unsigned char> buffer(4 * self->size.x * self->size.y);

    GLCALL(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data()))
        Tex2D_Save_Png(path, self->size.x, self->size.y, 4, buffer.data());
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0))
}

}

///
/// @brief ClipRect
///
namespace Ultra {

#define MAX_STACK_DEPTH 128

struct ClipRectTransform {
    float tx, ty;
    float sx, sy;
};

static ClipRectTransform transform[MAX_STACK_DEPTH];
static int transformIndex = -1;

struct ClipRectData {
    float x, y;
    float sx, sy;
    bool enabled;
};

static ClipRectData rect[MAX_STACK_DEPTH];
static int rectIndex = -1;

inline static void TransformRect(float *x, float *y, float *sx, float *sy) {
    if (transformIndex >= 0) {
        ClipRectTransform *curr = transform + transformIndex;
        *x = curr->sx * (*x) + curr->tx;
        *y = curr->sy * (*y) + curr->ty;
        *sx = curr->sx * (*sx);
        *sy = curr->sy * (*sy);
    }
}

void ClipRect_Activate(ClipRectData *self) {
    if (self && self->enabled) {
        Vec2i vpSize;
        Viewport::GetSize(&vpSize);
        GLCALL(glEnable(GL_SCISSOR_TEST))
            float x = self->x;
        float y = self->y;
        float sx = self->sx;
        float sy = self->sy;
        TransformRect(&x, &y, &sx, &sy);
        GLCALL(glScissor(
            (int)x,
            vpSize.y - (int)(y + sy),
            (int)sx,
            (int)sy))
    } else {
        GLCALL(glDisable(GL_SCISSOR_TEST))
    }
}

void ClipRect::Push(float x, float y, float sx, float sy) {
    if (rectIndex + 1 >= MAX_STACK_DEPTH)
        Fatal("ClipRect_Push: Maximum stack depth exceeded");
    rectIndex++;
    ClipRectData *curr = rect + rectIndex;
    curr->x = x;
    curr->y = y;
    curr->sx = sx;
    curr->sy = sy;
    curr->enabled = true;
    ClipRect_Activate(curr);
}

void ClipRect::PushCombined(float x, float y, float sx, float sy) {
    ClipRectData *curr = rect + rectIndex;
    if (rectIndex >= 0 && curr->enabled) {
        float maxX = x + sx;
        float maxY = y + sy;
        x = Max(x, curr->x);
        y = Max(y, curr->y);

        Push(x, y,
          Min(maxX, curr->x + curr->sx) - x,
          Min(maxY, curr->y + curr->sy) - y
        );
    } else {
        Push(x, y, sx, sy);
    }
}

void ClipRect::PushDisabled() {
    if (rectIndex + 1 >= MAX_STACK_DEPTH)
        Fatal("ClipRect_Push: Maximum stack depth exceeded");
    rectIndex++;
    ClipRectData *curr = rect + rectIndex;
    curr->enabled = false;
    ClipRect_Activate(curr);
}

void ClipRect::PushTransform(float tx, float ty, float sx, float sy) {
    if (transformIndex + 1 >= MAX_STACK_DEPTH)
        Fatal("ClipRect_PushTransform: Maximum stack depth exceeded");
    transformIndex++;
    ClipRectTransform *curr = transform + transformIndex;
    curr->tx = tx;
    curr->ty = ty;
    curr->sx = sx;
    curr->sy = sy;
    if (rectIndex >= 0)
        ClipRect_Activate(rect + rectIndex);
}

void ClipRect::Pop() {
    if (rectIndex < 0)
        Fatal("ClipRect_Pop: Attempting to pop an empty stack");
    rectIndex--;
    ClipRect_Activate(rectIndex >= 0 ? rect + rectIndex : 0);
}

void ClipRect::PopTransform() {
    if (transformIndex < 0)
        Fatal("ClipRect_PopTransform: Attempting to pop an empty stack");
    transformIndex--;
    if (rectIndex >= 0)
        ClipRect_Activate(rect + rectIndex);
}

}

///
/// @brief RenderState
///
namespace Ultra {
#include "BlendMode.h"
#include "CullFace.h"
#include "OpenGL.h"
#include "RenderState.h"

#define MAX_STACK_DEPTH 16

#define RENDERSTATE_X                                                          \
  X(PhxBlendMode, BlendMode, blendMode)                                        \
  X(PhxCullFace, CullFace, cullFace)                                           \
  X(bool, DepthTest, depthTest)                                                \
  X(bool, DepthWritable, depthWritable)                                        \
  X(bool, Wireframe, wireframe)

#define X(T, StateFn, State)                                                   \
  static T State[MAX_STACK_DEPTH];                                             \
  static int State##Index = -1;
RENDERSTATE_X
#undef X

inline static void RenderState_SetBlendMode(PhxBlendMode mode) {
    switch (mode) {
        case PhxBlendMode::Additive:
            GLCALL(glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE))
                return;
        case PhxBlendMode::Alpha:
            GLCALL(glBlendFuncSeparate(
                GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                GL_ONE, GL_ONE_MINUS_SRC_ALPHA))
                return;
        case PhxBlendMode::PreMultAlpha:
            GLCALL(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA))
                return;
        case PhxBlendMode::Disabled:
            GLCALL(glBlendFunc(GL_ONE, GL_ZERO))
                return;
    }
}

inline static void RenderState_SetCullFace(PhxCullFace mode) {
    switch (mode) {
        case PhxCullFace::None:
            GLCALL(glDisable(GL_CULL_FACE))
                return;
        case PhxCullFace::Back:
            GLCALL(glEnable(GL_CULL_FACE))
                GLCALL(glCullFace(GL_BACK))
                return;
        case PhxCullFace::Front:
            GLCALL(glEnable(GL_CULL_FACE))
                GLCALL(glCullFace(GL_FRONT))
                return;
    }
}

inline static void RenderState_SetDepthTest(bool enabled) {
    if (enabled)
        GLCALL(glEnable(GL_DEPTH_TEST))
    else
        GLCALL(glDisable(GL_DEPTH_TEST))
}

inline static void RenderState_SetDepthWritable(bool enabled) {
    GLCALL(glDepthMask(enabled))
}

inline static void RenderState_SetWireframe(bool enabled) {
    if (enabled)
        GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE))
    else
        GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL))
}

void RenderState::PushAllDefaults() {
    PushBlendMode(PhxBlendMode::Disabled);
    PushCullFace(PhxCullFace::None);
    PushDepthTest(false);
    PushDepthWritable(true);
    PushWireframe(false);
}

void RenderState::PopAll() {
    PopBlendMode();
    PopCullFace();
    PopDepthTest();
    PopDepthWritable();
    PopWireframe();
}

#define X(T, StateFn, State)                                                   \
  void RenderState::Push##StateFn(T value) {                                    \
    if (State##Index + 1 >= MAX_STACK_DEPTH)                                   \
      Fatal("RenderState::Push" #StateFn ": Maximum state stack depth exceeded"); \
    State[++State##Index] = value;                                             \
    RenderState_Set##StateFn(value);                                           \
  }                                                                            \
                                                                               \
  void RenderState::Pop##StateFn() {                                            \
    if (State##Index < 0)                                                      \
      Fatal("RenderState::Pop" #StateFn ": Attempting to pop an empty state stack"); \
    State##Index--;                                                            \
    if (State##Index >= 0)                                                     \
      RenderState_Set##StateFn(State[State##Index]);                           \
  }

RENDERSTATE_X
#undef X

}

///
/// @brief RenderTarget
///
namespace Ultra {

#define MAX_COLOR_ATTACHMENTS 4
#define MAX_STACK_DEPTH 16

struct FBO {
    uint handle;
    int colorIndex;
    int sx, sy;
    bool depth;
};

static int fboIndex = -1;
static FBO fboStack[MAX_STACK_DEPTH];

inline static FBO *GetActive() {
    return fboStack + fboIndex;
}

inline static void SetDrawBuffers(int count) {
    static const GLenum bufs[MAX_COLOR_ATTACHMENTS] = {
      GL_COLOR_ATTACHMENT0,
      GL_COLOR_ATTACHMENT1,
      GL_COLOR_ATTACHMENT2,
      GL_COLOR_ATTACHMENT3,
    };

    GLCALL(glDrawBuffers(count, bufs))
}

void RenderTarget::Push(int sx, int sy) {
    FRAME_BEGIN;
    if (fboIndex + 1 >= MAX_STACK_DEPTH)
        Fatal("RenderTarget_Push: Maximum stack depth exceeded");

    fboIndex++;
    FBO *self = GetActive();
    self->handle = 0;
    self->colorIndex = 0;
    self->sx = sx;
    self->sy = sy;
    self->depth = false;
    Metric_Inc(Metric_FBOSwap);
    GLCALL(glGenFramebuffers(1, &self->handle))
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, self->handle))
        Viewport::Push(0, 0, sx, sy, false);
    FRAME_END;
}

void RenderTarget::Pop() {
    FRAME_BEGIN;
    if (fboIndex < 0)
        Fatal("RenderTarget_Pop: Attempting to pop an empty stack");

      /* Delete current FBO. */
    for (int i = 0; i < MAX_COLOR_ATTACHMENTS; ++i)
        GLCALL(glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D, 0, 0))

        GLCALL(glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, 0, 0))

        GLCALL(glDeleteFramebuffers(1, &fboStack[fboIndex].handle))
        fboIndex--;

        /* Activate previous FBO. */
    Metric_Inc(Metric_FBOSwap);
    if (fboIndex >= 0)
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, GetActive()->handle))
    else
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0))

        Viewport::Pop();
    FRAME_END;
}

void RenderTarget::BindTex2D(Tex2DData *self) {
    RenderTarget::BindTex2DLevel(self, 0);
}

/* TODO : Check whether it is valid to bind a mip level of a depth buffer?
 *        (Do depth formats even have levels??) */
void RenderTarget::BindTex2DLevel(Tex2DData *tex, int level) {
    FBO *self = GetActive();
    uint handle = Tex2D::GetHandle(tex);

    /* Color attachment. */
    if (IsPhxTextureFormatColor(Tex2D::GetFormat(tex))) {
        if (self->colorIndex >= MAX_COLOR_ATTACHMENTS)
            Fatal("RenderTarget_BindTex2D: Max color attachments exceeded");
        GLCALL(glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + self->colorIndex++,
            GL_TEXTURE_2D, handle, level))
            SetDrawBuffers(self->colorIndex);
    }

    /* Depth attachment. */
    else {
        if (self->depth)
            Fatal("RenderTarget_BindTex2D: Target already has a depth buffer");
        GLCALL(glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, handle, level))
            self->depth = true;
    }
}

//void RenderTarget::BindTex3D(Tex3D *self, int layer) {
//    RenderTarget_BindTex3DLevel(self, layer, 0);
//}
//
//void RenderTarget::BindTex3DLevel(Tex3D *tex, int layer, int level) {
//    FBO *self = GetActive();
//    if (self->colorIndex >= MAX_COLOR_ATTACHMENTS)
//        Fatal("RenderTarget_BindTex3D: Max color attachments exceeded");
//
//    uint handle = Tex3D_GetHandle(tex);
//    GLCALL(glFramebufferTexture3D(
//        GL_FRAMEBUFFER,
//        GL_COLOR_ATTACHMENT0 + self->colorIndex++,
//        GL_TEXTURE_3D, handle, level, layer))
//        SetDrawBuffers(self->colorIndex);
//}
//
//void RenderTarget::BindTexCube(TexCube *self, CubeFace face) {
//    RenderTarget_BindTexCubeLevel(self, face, 0);
//}
//
//void RenderTarget::BindTexCubeLevel(TexCube *tex, CubeFace face, int level) {
//    FBO *self = GetActive();
//    if (self->colorIndex >= MAX_COLOR_ATTACHMENTS)
//        Fatal("RenderTarget_BindTexCubeLevel: Max color attachments exceeded");
//
//    uint handle = TexCube_GetHandle(tex);
//    GLCALL(glFramebufferTexture2D(
//        GL_FRAMEBUFFER,
//        GL_COLOR_ATTACHMENT0 + self->colorIndex++,
//        face, handle, level))
//        SetDrawBuffers(self->colorIndex);
//}

void RenderTarget::PushTex2D(Tex2DData *self) {
    PushTex2DLevel(self, 0);
}

void RenderTarget::PushTex2DLevel(Tex2DData *self, int level) {
    Vec2i size; Tex2D::GetSizeLevel(self, &size, level);
    Push(size.x, size.y);
    BindTex2DLevel(self, level);
}

//void RenderTarget::PushTex3D(Tex3D *self, int layer) {
//    RenderTarget_PushTex3DLevel(self, layer, 0);
//}
//
//void RenderTarget::PushTex3DLevel(Tex3D *self, int layer, int level) {
//    Vec3i size; Tex3D_GetSizeLevel(self, &size, level);
//    RenderTarget_Push(size.x, size.y);
//    RenderTarget_BindTex3DLevel(self, layer, level);
//}

}

///
/// @brief Draw
///
namespace Ultra {
#define MAX_STACK_DEPTH 16

static float alphaStack[MAX_STACK_DEPTH];
static int alphaIndex = -1;
static Vec4f color = { 1, 1, 1, 1 };


void Draw::PushAlpha(float a) {
    if (alphaIndex + 1 >= MAX_STACK_DEPTH)
        Fatal("Draw_PushAlpha: Maximum alpha stack depth exceeded");

    float prevAlpha = alphaIndex >= 0 ? alphaStack[alphaIndex] : 1;
    float alpha = a * prevAlpha;
    alphaStack[++alphaIndex] = alpha;
    GLCALL(glColor4f(color.x, color.y, color.z, color.w * alpha));
}

void Draw::PopAlpha() {
    if (alphaIndex < 0)
        Fatal("Draw_PopAlpha Attempting to pop an empty alpha stack");

    alphaIndex--;
    float alpha = alphaIndex >= 0 ? alphaStack[alphaIndex] : 1;
    GLCALL(glColor4f(color.x, color.y, color.z, color.w * alpha));
}

void Draw::Axes(Vec3f const *pos, Vec3f const *x, Vec3f const *y, Vec3f const *z, float scale, float _alpha) {
    Vec3f left = Vec3f_Add(*pos, Vec3f_Muls(*x, scale));
    Vec3f up = Vec3f_Add(*pos, Vec3f_Muls(*y, scale));
    Vec3f forward = Vec3f_Add(*pos, Vec3f_Muls(*z, scale));
    glBegin(GL_LINES);
    glColor4f(1, 0.25f, 0.25f, _alpha);
    glVertex3f(UNPACK3(*pos));
    glVertex3f(UNPACK3(left));
    glColor4f(0.25f, 1, 0.25f, _alpha);
    glVertex3f(UNPACK3(*pos));
    glVertex3f(UNPACK3(up));
    glColor4f(0.25f, 0.25f, 1, _alpha);
    glVertex3f(UNPACK3(*pos));
    glVertex3f(UNPACK3(forward));
    GLCALL(glEnd())

        glBegin(GL_POINTS);
    glColor4f(1, 1, 1, _alpha);
    glVertex3f(UNPACK3(*pos));
    GLCALL(glEnd())
}

void Draw::Border(float s, float x, float y, float w, float h) {
    Draw::Rect(x, y, w, s);
    Draw::Rect(x, y + h - s, w, s);
    Draw::Rect(x, y + s, s, h - 2 * s);
    Draw::Rect(x + w - s, y + s, s, h - 2 * s);
}

void Draw::Box3(Box3f const *self) {
    Metric_AddDrawImm(6, 12, 24);
    glBegin(GL_QUADS);
    /* Left. */
    glVertex3f(self->lower.x, self->lower.y, self->lower.z);
    glVertex3f(self->lower.x, self->lower.y, self->upper.z);
    glVertex3f(self->lower.x, self->upper.y, self->upper.z);
    glVertex3f(self->lower.x, self->upper.y, self->lower.z);

    /* Right. */
    glVertex3f(self->upper.x, self->lower.y, self->lower.z);
    glVertex3f(self->upper.x, self->upper.y, self->lower.z);
    glVertex3f(self->upper.x, self->upper.y, self->upper.z);
    glVertex3f(self->upper.x, self->lower.y, self->upper.z);

    /* Front. */
    glVertex3f(self->lower.x, self->lower.y, self->upper.z);
    glVertex3f(self->upper.x, self->lower.y, self->upper.z);
    glVertex3f(self->upper.x, self->upper.y, self->upper.z);
    glVertex3f(self->lower.x, self->upper.y, self->upper.z);

    /* Back. */
    glVertex3f(self->lower.x, self->lower.y, self->lower.z);
    glVertex3f(self->lower.x, self->upper.y, self->lower.z);
    glVertex3f(self->upper.x, self->upper.y, self->lower.z);
    glVertex3f(self->upper.x, self->lower.y, self->lower.z);

    /* Top. */
    glVertex3f(self->lower.x, self->upper.y, self->lower.z);
    glVertex3f(self->lower.x, self->upper.y, self->upper.z);
    glVertex3f(self->upper.x, self->upper.y, self->upper.z);
    glVertex3f(self->upper.x, self->upper.y, self->lower.z);

    /* Bottom. */
    glVertex3f(self->lower.x, self->lower.y, self->lower.z);
    glVertex3f(self->upper.x, self->lower.y, self->lower.z);
    glVertex3f(self->upper.x, self->lower.y, self->upper.z);
    glVertex3f(self->lower.x, self->lower.y, self->upper.z);
    GLCALL(glEnd())
}

void Draw::Clear(float r, float g, float b, float a) {
    GLCALL(glClearColor(r, g, b, a))
        GLCALL(glClear(GL_COLOR_BUFFER_BIT))
}

void Draw::ClearDepth(float d) {
    GLCALL(glClearDepth(d))
        GLCALL(glClear(GL_DEPTH_BUFFER_BIT))
}

void Draw::Color(float r, float g, float b, float a) {
    float alpha = alphaIndex >= 0 ? alphaStack[alphaIndex] : 1;
    color = Vec4f_Create(r, g, b, a);
    GLCALL(glColor4f(r, g, b, a * alpha))
}

void Draw::Flush() {
    Metric_Inc(Metric_Flush);
    GLCALL(glFinish())
}

void Draw::Line(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    GLCALL(glEnd())
}

void Draw::Line3(Vec3f const *p1, Vec3f const *p2) {
    glBegin(GL_LINES);
    glVertex3f(UNPACK3(*p1));
    glVertex3f(UNPACK3(*p2));
    GLCALL(glEnd())
}

void Draw::LineWidth(float width) {
    GLCALL(glLineWidth(width))
}

void Draw::Plane(Vec3f const *p, Vec3f const *n, float scale) {
    Vec3f e1 = Abs(n->x) < 0.7f ? Vec3f_Create(1, 0, 0) : Vec3f_Create(0, 1, 0);
    e1 = Vec3f_Normalize(Vec3f_Reject(e1, *n));
    Vec3f e2 = Vec3f_Cross(*n, e1);

    Vec3f p0 = Vec3f_Add(*p, Vec3f_Add(Vec3f_Muls(e1, -scale), Vec3f_Muls(e2, -scale)));
    Vec3f p1 = Vec3f_Add(*p, Vec3f_Add(Vec3f_Muls(e1, scale), Vec3f_Muls(e2, -scale)));
    Vec3f p2 = Vec3f_Add(*p, Vec3f_Add(Vec3f_Muls(e1, scale), Vec3f_Muls(e2, scale)));
    Vec3f p3 = Vec3f_Add(*p, Vec3f_Add(Vec3f_Muls(e1, -scale), Vec3f_Muls(e2, scale)));

    Metric_AddDrawImm(1, 2, 4);
    glBegin(GL_QUADS);
    glVertex3f(UNPACK3(p0));
    glVertex3f(UNPACK3(p1));
    glVertex3f(UNPACK3(p2));
    glVertex3f(UNPACK3(p3));
    GLCALL(glEnd())
}

void Draw::Point(float x, float y) {
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    GLCALL(glEnd())
}

void Draw::Point3(float x, float y, float z) {
    glBegin(GL_POINTS);
    glVertex3f(x, y, z);
    GLCALL(glEnd())
}

void Draw::PointSize(float size) {
    GLCALL(glPointSize(size))
}

void Draw::Poly(Vec2f const *points, int count) {
    Metric_AddDrawImm(1, count - 2, count);
    glBegin(GL_POLYGON);
    for (int i = 0; i < count; ++i)
        glVertex2f(UNPACK2(points[i]));
    GLCALL(glEnd());
}

void Draw::Poly3(Vec3f const *points, int count) {
    Metric_AddDrawImm(1, count - 2, count);
    glBegin(GL_POLYGON);
    for (int i = 0; i < count; ++i)
        glVertex3f(UNPACK3(points[i]));
    GLCALL(glEnd());
}

void Draw::Quad(Vec2f const *p1, Vec2f const *p2, Vec2f const *p3, Vec2f const *p4) {
    Metric_AddDrawImm(1, 2, 4);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(UNPACK2(*p1));
    glTexCoord2f(0, 1); glVertex2f(UNPACK2(*p2));
    glTexCoord2f(1, 1); glVertex2f(UNPACK2(*p3));
    glTexCoord2f(1, 0); glVertex2f(UNPACK2(*p4));
    GLCALL(glEnd())
}

void Draw::Quad3(Vec3f const *p1, Vec3f const *p2, Vec3f const *p3, Vec3f const *p4) {
    Metric_AddDrawImm(1, 2, 4);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(UNPACK3(*p1));
    glTexCoord2f(0, 1); glVertex3f(UNPACK3(*p2));
    glTexCoord2f(1, 1); glVertex3f(UNPACK3(*p3));
    glTexCoord2f(1, 0); glVertex3f(UNPACK3(*p4));
    GLCALL(glEnd())
}

void Draw::Rect(float x1, float y1, float xs, float ys) {
    float x2 = x1 + xs;
    float y2 = y1 + ys;
    Metric_AddDrawImm(1, 2, 4);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x1, y1);
    glTexCoord2f(0, 1); glVertex2f(x1, y2);
    glTexCoord2f(1, 1); glVertex2f(x2, y2);
    glTexCoord2f(1, 0); glVertex2f(x2, y1);
    GLCALL(glEnd())
}

void Draw::SmoothLines(bool enabled) {
    if (enabled) {
        GLCALL(glEnable(GL_LINE_SMOOTH))
            GLCALL(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST))
    } else {
        GLCALL(glDisable(GL_LINE_SMOOTH))
            GLCALL(glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST))
    }
}

void Draw::SmoothPoints(bool enabled) {
    if (enabled) {
        GLCALL(glEnable(GL_POINT_SMOOTH))
            GLCALL(glHint(GL_POINT_SMOOTH_HINT, GL_NICEST))
    } else {
        GLCALL(glDisable(GL_POINT_SMOOTH))
            GLCALL(glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST))
    }
}

inline static Vec3f Spherical(float r, float yaw, float pitch) {
    return Vec3f_Create(
      r * Sin(pitch) * Cos(yaw),
      r * Cos(pitch),
      r * Sin(pitch) * Sin(yaw));
}

/* TODO JP : Lazy creation of VBO / IBO & glDraw instead of immediate. */
void Draw::Sphere(Vec3f const *p, float r) {
    const size_t res = 7;
    const float fRes = float(res);

    /* First Row */ {
        Metric_AddDrawImm(res, res, res * 3);
        glBegin(GL_TRIANGLES);
        float lastTheta = float(res - 1) / fRes * Tau;
        float phi = 1.0f / fRes * Pi;
        Vec3f tc = Vec3f_Add(*p, Spherical(r, 0, 0));
        for (size_t iTheta = 0; iTheta < res; iTheta++) {
            float theta = float(iTheta) / fRes * Tau;
            Vec3f br = Vec3f_Add(*p, Spherical(r, lastTheta, phi));
            Vec3f bl = Vec3f_Add(*p, Spherical(r, theta, phi));
            glVertex3f(UNPACK3(br));
            glVertex3f(UNPACK3(tc));
            glVertex3f(UNPACK3(bl));
            lastTheta = theta;
        }
        GLCALL(glEnd())
    }

    /* Middle Rows */ {
        Metric_AddDrawImm(res - 2, 2 * (res - 2), 4 * (res - 2));
        glBegin(GL_QUADS);
        float lastPhi = 1.0f / fRes * Pi;
        float lastTheta = float(res - 1) / fRes * Tau;

        for (size_t iPhi = 2; iPhi < res; iPhi++) {
            float phi = float(iPhi) / fRes * Pi;
            for (size_t iTheta = 0; iTheta < res; iTheta++) {
                float theta = float(iTheta) / fRes * Tau;
                Vec3f br = Vec3f_Add(*p, Spherical(r, lastTheta, phi));
                Vec3f tr = Vec3f_Add(*p, Spherical(r, lastTheta, lastPhi));
                Vec3f tl = Vec3f_Add(*p, Spherical(r, theta, lastPhi));
                Vec3f bl = Vec3f_Add(*p, Spherical(r, theta, phi));
                glVertex3f(UNPACK3(br));
                glVertex3f(UNPACK3(tr));
                glVertex3f(UNPACK3(tl));
                glVertex3f(UNPACK3(bl));
                lastTheta = theta;
            }
            lastPhi = phi;
        }
        GLCALL(glEnd())
    }

    /* Bottom Row */ {
        Metric_AddDrawImm(res, res, res * 3);
        glBegin(GL_TRIANGLES);
        float lastTheta = float(res - 1) / fRes * Tau;
        float phi = float(res - 1) / fRes * Pi;
        Vec3f bc = Vec3f_Add(*p, Spherical(r, 0, Pi));

        for (size_t iTheta = 0; iTheta < res; iTheta++) {
            float theta = float(iTheta) / fRes * Tau;
            Vec3f tr = Vec3f_Add(*p, Spherical(r, lastTheta, phi));
            Vec3f tl = Vec3f_Add(*p, Spherical(r, theta, phi));
            glVertex3f(UNPACK3(tr));
            glVertex3f(UNPACK3(tl));
            glVertex3f(UNPACK3(bc));
            lastTheta = theta;
        }
        GLCALL(glEnd())
    }
}

void Draw::Tri(Vec2f const *v1, Vec2f const *v2, Vec2f const *v3) {
    Metric_AddDrawImm(1, 1, 3);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0); glVertex2f(UNPACK2(*v1));
    glTexCoord2f(0, 1); glVertex2f(UNPACK2(*v2));
    glTexCoord2f(1, 1); glVertex2f(UNPACK2(*v3));
    GLCALL(glEnd())
}

void Draw::Tri3(Vec3f const *v1, Vec3f const *v2, Vec3f const *v3) {
    Metric_AddDrawImm(1, 1, 3);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0); glVertex3f(UNPACK3(*v1));
    glTexCoord2f(0, 1); glVertex3f(UNPACK3(*v2));
    glTexCoord2f(1, 1); glVertex3f(UNPACK3(*v3));
    GLCALL(glEnd())
}

}

///
/// @brief Viewport
///
namespace Ultra {

#define MAX_STACK_DEPTH 16

/* TODO : This is a low-level mechanism and probably not for use outside of
 *        RenderTarget. Should likely be folded into RenderTarget. */

struct VP {
    int x, y, sx, sy;
    bool isWindow;
};

static int vpIndex = -1;
static VP vp[MAX_STACK_DEPTH];

static void Viewport_Set(VP const *self) {
    GLCALL(glViewport(self->x, self->y, self->sx, self->sy))
        GLCALL(glMatrixMode(GL_PROJECTION))
        GLCALL(glLoadIdentity())

        /* GL's window coordinates and texture coordinates have opposite vertical
         * orientation. Automatically compensate via the projection matrix. */
        if (self->isWindow) {
            GLCALL(glTranslatef(-1.0, 1.0, 0.0))
                GLCALL(glScalef(2.0f / self->sx, -2.0f / self->sy, 1.0f))
        } else {
            GLCALL(glTranslatef(-1.0, -1.0, 0.0))
                GLCALL(glScalef(2.0f / self->sx, 2.0f / self->sy, 1.0f))
        }
}

float Viewport::GetAspect() {
    if (vpIndex < 0)
        Fatal("Viewport_GetAspect: Viewport stack is empty");
    return (float)vp[vpIndex].sx / (float)vp[vpIndex].sy;
}

void Viewport::GetSize(Vec2i *out) {
    if (vpIndex < 0)
        Fatal("Viewport_GetSize: Viewport stack is empty");
    out->x = vp[vpIndex].sx;
    out->y = vp[vpIndex].sy;
}

void Viewport::Push(int x, int y, int sx, int sy, bool isWindow) {
    if (vpIndex + 1 >= MAX_STACK_DEPTH)
        Fatal("Viewport_Push: Maximum viewport stack depth exceeded");
    vpIndex++;
    VP *self = vp + vpIndex;
    self->x = x;
    self->y = y;
    self->sx = sx;
    self->sy = sy;
    self->isWindow = isWindow;
    Viewport_Set(self);
}

void Viewport::Pop() {
    if (vpIndex < 0)
        Fatal("Viewport_Pop: Viewport stack is empty");
    vpIndex--;
    if (vpIndex >= 0)
        Viewport_Set(vp + vpIndex);
}

}

///
/// @brief Window
///
namespace Ultra {

struct WindowData {
    SDL_Window *handle;
    SDL_GLContext context;
    int32_t mode;
};

WindowData *PhxWindow::Create(cstr title, PhxWindowPosition x, PhxWindowPosition y, int sx, int sy, WindowMode mode) {
    WindowData *self = MemNew(WindowData);
    int32_t nativeMode = (int32_t)mode;
    nativeMode |= SDL_WINDOW_OPENGL;
    nativeMode |= (int32_t)WindowMode::Resizable;
    self->handle = SDL_CreateWindow(title, (int)x, (int)y, sx, sy, (unsigned int)nativeMode);
    self->context = SDL_GL_CreateContext(self->handle);
    auto result = SDL_GetError();
    self->mode = nativeMode;
    if (!self->context) Fatal("Failed to create OpenGL context for window");
    OpenGL_Init();
    return self;
}

void PhxWindow::Free(WindowData *self) {
    SDL_GL_DeleteContext(self->context);
    SDL_DestroyWindow(self->handle);
    MemFree(self);
}

void PhxWindow::BeginDraw(WindowData *self) {
    Vec2i size;
    SDL_GL_MakeCurrent(self->handle, self->context);
    PhxWindow::GetSize(self, &size);
    Viewport::Push(0, 0, size.x, size.y, true);
}

void PhxWindow::EndDraw(WindowData *self) {
    Viewport::Pop();
    SDL_GL_SwapWindow(self->handle);
}

void PhxWindow::GetSize(WindowData *self, Vec2i *out) {
    SDL_GetWindowSize(self->handle, &out->x, &out->y);
}

void PhxWindow::GetPosition(WindowData *self, Vec2i *out) {
    SDL_GetWindowPosition(self->handle, &out->x, &out->y);
}

cstr PhxWindow::GetTitle(WindowData *self) {
    return SDL_GetWindowTitle(self->handle);
}

void PhxWindow::SetFullscreen(WindowData *self, bool fs) {
    SDL_SetWindowFullscreen(self->handle, fs ? (uint32_t)WindowMode::Fullscreen : 0);
}

void PhxWindow::SetPosition(WindowData *self, PhxWindowPosition x, PhxWindowPosition y) {
    SDL_SetWindowPosition(self->handle, (int)x, (int)y);
}

void PhxWindow::SetSize(WindowData *self, int sx, int sy) {
    SDL_SetWindowSize(self->handle, sx, sy);
}

void PhxWindow::SetTitle(WindowData *self, cstr title) {
    SDL_SetWindowTitle(self->handle, title);
}

void PhxWindow::SetVsync(WindowData *, bool vsync) {
    SDL_GL_SetSwapInterval(vsync ? 1 : 0);
}

void PhxWindow::ToggleFullscreen(WindowData *self) {
    if (self->mode & (int)WindowMode::Fullscreen)
        SDL_SetWindowFullscreen(self->handle, 0);
    else
        SDL_SetWindowFullscreen(self->handle, (int)WindowMode::Fullscreen);
    self->mode ^= (int32_t)WindowMode::Fullscreen;
}

void PhxWindow::Hide(WindowData *self) {
    SDL_HideWindow(self->handle);
}

void PhxWindow::Show(WindowData *self) {
    SDL_ShowWindow(self->handle);
}

}
