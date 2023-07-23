module;

#include <glad/gl.h>

// ToDo: Check Profiler
#define FRAME_BEGIN
#define FRAME_END

#include "Vec2.h"
#include "Vec4.h"

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

import Ultra.System.FileSystem;
import Ultra.Utility.String;

///
/// @brief Resource
///
namespace Ultra {

using PathList = std::list<string>;
static unordered_map<ResourceType, PathList> sPaths;

Resource::Resource() {
    Resource::AddPath(ResourceType::Font, "./Assets/Phoenix/res/font/{}.ttf");
    Resource::AddPath(ResourceType::Font, "./Assets/Phoenix/res/font/{}.otf");
    Resource::AddPath(ResourceType::Mesh, "./Assets/Phoenix/res/mesh/{}.bin");
    Resource::AddPath(ResourceType::Mesh, "./Assets/Phoenix/res/mesh/{}.obj");
    Resource::AddPath(ResourceType::Script, "./Assets/Phoenix/res/script/{}.lua");
    Resource::AddPath(ResourceType::Shader, "./Assets/Phoenix/res/shader/{}.glsl");
    Resource::AddPath(ResourceType::Sound, "./Assets/Phoenix/res/sound/{}.mp3");
    Resource::AddPath(ResourceType::Sound, "./Assets/Phoenix/res/sound/{}.ogg");
    Resource::AddPath(ResourceType::Sound, "./Assets/Phoenix/res/sound/{}.ogx");
    Resource::AddPath(ResourceType::Sound, "./Assets/Phoenix/res/sound/{}.wav");
    Resource::AddPath(ResourceType::Tex1D, "./Assets/Phoenix/res/tex1d/{}.bin");
    Resource::AddPath(ResourceType::Tex2D, "./Assets/Phoenix/res/tex2d/{}.jpg");
    Resource::AddPath(ResourceType::Tex2D, "./Assets/Phoenix/res/tex2d/{}.png");
    Resource::AddPath(ResourceType::Tex3D, "./Assets/Phoenix/res/tex3d/{}.bin");
    Resource::AddPath(ResourceType::TexCube, "./Assets/Phoenix/res/texcube/{}");
    Resource::AddPath(ResourceType::Other, "./Assets/Phoenix/res/{}");
}

Resource::~Resource() {
}

bool Resource::Exists(ResourceType type, const string &name) {
    return Resolve(type, name).empty() ? false : true;
}

string Resource::GetPath(ResourceType type, const string &name) {
    return Resolve(type, name);
}

vector<uint32_t> Resource::LoadBytes(ResourceType type, const string &name) {
    string path = Resolve(type, name);
    auto data = ReadFileBinary(path.c_str());
    if (data.empty()) Fatal("Resource_LoadBytes: Failed to load %s <%s> at <%s>", Resource::ToString(type), name, path);
    return data;
}

string Resource::LoadCstr(ResourceType type, const string &name) {
    string path = Resolve(type, name);
    string data = ReadFile(path);
    if (data.empty()) Fatal("Resource_LoadCstr: Failed to load %s <%s> at <%s>", Resource::ToString(type), name, path);
    return data;
}

void Resource::AddPath(ResourceType type, const string &format) {
    sPaths[type].push_back({ format });
}

string Resource::Resolve(ResourceType type, const string &name) {
    auto entries = sPaths[type];
    if (!entries.empty()) {
        for (auto &entry : entries) {
            auto resource = std::vformat(entry, std::make_format_args(name));
            if (!resource.empty()) {
                if (FileSystemObjectExists(resource)) {
                    return resource;
                }
            }
        }
    }

    // Search literal path
    if (FileSystemObjectExists(name)) {
        return name;
    }

    LogWarning("Could not resolve path of {} '{}'!", ToString(type), name);
    return {};
}

string Resource::ToString(ResourceType type) {
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

}

///
/// @brief Metrics
///
namespace Ultra {

static int32 valueCurr[(int)Metrics::SIZE + 1] = { 0 };

int32 Metric::Get(Metrics self) {
    return valueCurr[(int)self];
}

cstr Metric::GetName(Metrics self) {
    switch (self) {
        case Metrics::DrawCalls:  return "Draw Calls";
        case Metrics::Immediate:  return "Draw Calls (Immediate)";
        case Metrics::PolysDrawn: return "Polys";
        case Metrics::TrisDrawn:  return "Tris";
        case Metrics::VertsDrawn: return "Vertices";
        case Metrics::Flush:      return "Pipeline Flushes";
        case Metrics::FBOSwap:    return "Framebuffer Swaps";
    }
    return 0;
}

void Metric::AddDraw(int32 polys, int32 tris, int32 verts) {
    valueCurr[(int)Metrics::DrawCalls] += 1;
    valueCurr[(int)Metrics::PolysDrawn] += polys;
    valueCurr[(int)Metrics::TrisDrawn] += tris;
    valueCurr[(int)Metrics::VertsDrawn] += verts;
}

void Metric::AddDrawImm(int32 polys, int32 tris, int32 verts) {
    valueCurr[(int)Metrics::Immediate] += 1;
    valueCurr[(int)Metrics::PolysDrawn] += polys;
    valueCurr[(int)Metrics::TrisDrawn] += tris;
    valueCurr[(int)Metrics::VertsDrawn] += verts;
}

void Metric::Inc(Metrics self) {
    valueCurr[(int)self] += 1;
}

void Metric::Mod(Metrics self, int32 delta) {
    valueCurr[(int)self] += delta;
}

void Metric::Reset() {
    memset(valueCurr, 0, sizeof(valueCurr));
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
        if (string(s) == string(ShaderVarTypeGetGLSLName((ShaderVarType)i))) return (ShaderVarType)i;
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
    string *name {};
    uint vs;
    uint fs;
    uint program;
    uint texIndex;
    vector<ShaderVar> vars;
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

    glShaderSource(self, 2, srcs, 0);
    glCompileShader(self);

    /* Check for compile errors. */ {
    int status;
    glGetShaderiv(self, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            int length;
            glGetShaderiv(self, GL_INFO_LOG_LENGTH, &length);
            char *infoLog = (char *)calloc(1, length + 1);
            glGetShaderInfoLog(self, length, 0, infoLog);
            Fatal("CreateGLShader: Failed to compile shader:\n%s", infoLog);
        }
    }
    return self;
}

static uint CreateGLProgram(uint vs, uint fs) {
    uint self = glCreateProgram();
    glAttachShader(self, vs);
    glAttachShader(self, fs);

    /* TODO : Replace with custom directives. */ {
        glBindAttribLocation(self, 0, "vertex_position");
        glBindAttribLocation(self, 1, "vertex_normal");
        glBindAttribLocation(self, 2, "vertex_uv");
    }

    glLinkProgram(self);

        /* Check for link errors. */ {
        int status;
        glGetProgramiv(self, GL_LINK_STATUS, &status);
            if (status == GL_FALSE) {
                int length;
                glGetProgramiv(self, GL_INFO_LOG_LENGTH, &length);
                char *infoLog = (char *)calloc(1, length + 1);
                glGetProgramInfoLog(self, length, 0, infoLog);
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
    string rawCode = Resource::LoadCstr(ResourceType::Shader, name);
    cstr code = strdup(String::Replace(rawCode, "\r\n", "\n").c_str());
    code = GLSL_Preprocess(code, self);
    /* BUG : Disable GLSL caching until preprocessor cache works. */
    // StrMap_Set(cache, name, (void*)code);
    return code;
}

static cstr GLSL_Preprocess(cstr code, ShaderData *self) {
    const string includeString = "#include";
    const size_t lenInclude = includeString.length();

    cstr begin;

    /* Parse Includes. */
    while ((begin = strstr(code, "#include")) != 0) {
        cstr end = strstr(begin, "\n");
        string name(begin + lenInclude + 1, end);
        string path = includePath + name;
        //code = StrSub(code, begin, end, GLSL_Load(path.c_str(), self));
        string codeStr = code;
        string replaced = GLSL_Load(path.c_str(), self);

        codeStr.replace(begin - code, end - begin, replaced);
        code = strdup(codeStr.c_str());
    }

    /* Parse automatic ShaderVar stack bindings. */
    while ((begin = strstr(code, "#autovar")) != 0) {
        cstr end = strstr(begin, "\n");
        string line(begin, end);
        char varType[32] = { 0 };
        char varName[32] = { 0 };

        if (sscanf(line.c_str(), "#autovar %31s %31s", varType, varName) == 2) {
            ShaderVar var = {};
            var.type = ShaderVarTypeFromStr(varType);
            if (var.type == ShaderVarType::None)
                Fatal("GLSL_Preprocess: Unknown shader variable type <%s> "
                      "in directive:\n  %s", varType, line);
            var.name = strdup(varName);
            var.index = -1;
            self->vars.push_back(var);
        } else {
            Fatal("GLSL_Preprocess: Failed to parse directive:\n  %s", line.c_str());
        }

        string codeStr = code;
        codeStr.replace(begin - code, end - begin, "");
        code = strdup(codeStr.c_str());
    }
    return code;
}

static void Shader_BindVariables(ShaderData *self) {
    for (int i = 0; i < self->vars.size(); ++i) {
        ShaderVar *var = &self->vars[i];
        var->index = glGetUniformLocation(self->program, var->name);
        if (var->index < 0)
            Warn("Shader_BindVariables: Automatic shader variable <%s> does not exist"
                 " in shader <%s>", var->name, self->name);
    }
}

/* --- Creation ------------------------------------------------------------- */

ShaderData *PhxShader::Create(cstr vs, cstr fs) {
    ShaderData *self = new ShaderData();
    vs = GLSL_Preprocess(strdup(vs), self);
    fs = GLSL_Preprocess(strdup(fs), self);
    self->vs = CreateGLShader(vs, GL_VERTEX_SHADER);
    self->fs = CreateGLShader(fs, GL_FRAGMENT_SHADER);
    self->program = CreateGLProgram(self->vs, self->fs);
    self->texIndex = 1;
    self->name = new string(std::format("[anonymous shader @ {}]", reinterpret_cast<intptr_t>(self)));
    Shader_BindVariables(self);
    return self;
}

ShaderData *PhxShader::Load(cstr vName, cstr fName) {
    ShaderData *self = new ShaderData();
    cstr vs = GLSL_Load(vName, self);
    cstr fs = GLSL_Load(fName, self);
    self->vs = CreateGLShader(vs, GL_VERTEX_SHADER);
    self->fs = CreateGLShader(fs, GL_FRAGMENT_SHADER);
    self->program = CreateGLProgram(self->vs, self->fs);
    self->texIndex = 1;
    self->name = new string(std::format("[vs: {} , fs: {}]", vName, fName));
    Shader_BindVariables(self);
    return self;
}

void PhxShader::Acquire(ShaderData *self) {
    //RefCounted_Acquire(self);
}

void PhxShader::Free(ShaderData *self) {
    {
        glDeleteShader(self->vs);
        glDeleteShader(self->fs);
        glDeleteProgram(self->program);
        self->vars.clear();
        free((void*)self);
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
    glUseProgram(self->program);
        current = self;
    self->texIndex = 1;

    /* Fetch & bind automatic variables from the shader var stack. */
    for (int i = 0; i < self->vars.size(); ++i) {
        ShaderVar *var = &self->vars[i];
        if (var->index < 0) continue;
        void *pValue = PhxShaderVar::Get(var->name, var->type);
        if (!pValue)
            Fatal("Shader_Start: Shader variable stack does not contain variable <%s>", var->name);

        switch (var->type) {
            case ShaderVarType::Float: {
                float value = *(float *)pValue;
                glUniform1f(var->index, value);
                break; }
            case ShaderVarType::Float2: {
                Vec2f value = *(Vec2f *)pValue;
                glUniform2f(var->index, value.x, value.y);
                break; }
            case ShaderVarType::Float3: {
                Vec3f value = *(Vec3f *)pValue;
                glUniform3f(var->index, value.x, value.y, value.z);
                break; }
            case ShaderVarType::Float4: {
                Vec4f value = *(Vec4f *)pValue;
                glUniform4f(var->index, value.x, value.y, value.z, value.w);
                break; }
            case ShaderVarType::Int: {
                int value = *(int *)pValue;
                glUniform1i(var->index, value);
                break; }
            case ShaderVarType::Int2: {
                Vec2i value = *(Vec2i *)pValue;
                glUniform2i(var->index, value.x, value.y);
                break; }
            case ShaderVarType::Int3: {
                Vec3i value = *(Vec3i *)pValue;
                glUniform3i(var->index, value.x, value.y, value.z);
                break; }
            case ShaderVarType::Int4: {
                Vec4i value = *(Vec4i *)pValue;
                glUniform4i(var->index, value.x, value.y, value.z, value.w);
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
    glUseProgram(0);
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
    glUniform1f(GetUniformIndex(current, name), value);
}

void PhxShader::ISetFloat(int index, float value) {
    glUniform1f(index, value);
}

void PhxShader::SetFloat2(cstr name, float x, float y) {
    glUniform2f(GetUniformIndex(current, name), x, y);
}

void PhxShader::ISetFloat2(int index, float x, float y) {
    glUniform2f(index, x, y);
}

void PhxShader::SetFloat3(cstr name, float x, float y, float z) {
    glUniform3f(GetUniformIndex(current, name), x, y, z);
}

void PhxShader::ISetFloat3(int index, float x, float y, float z) {
    glUniform3f(index, x, y, z);
}

void PhxShader::SetFloat4(cstr name, float x, float y, float z, float w) {
    glUniform4f(GetUniformIndex(current, name), x, y, z, w);
}

void PhxShader::ISetFloat4(int index, float x, float y, float z, float w) {
    glUniform4f(index, x, y, z, w);
}

void PhxShader::SetInt(cstr name, int value) {
    glUniform1i(GetUniformIndex(current, name), value);
}

void PhxShader::ISetInt(int index, int value) {
    glUniform1i(index, value);
}

void PhxShader::SetMatrix(cstr name, Matrix *value) {
    glUniformMatrix4fv(GetUniformIndex(current, name), 1, true, (float *)value);
}

void PhxShader::SetMatrixT(cstr name, Matrix *value) {
    glUniformMatrix4fv(GetUniformIndex(current, name), 1, false, (float *)value);
}

void PhxShader::ISetMatrix(int index, Matrix *value) {
    glUniformMatrix4fv(index, 1, true, (float *)value);
}

void PhxShader::ISetMatrixT(int index, Matrix *value) {
    glUniformMatrix4fv(index, 1, false, (float *)value);
}

//void PhxShader::SetTex1D(cstr name, Tex1D *value) {
//    glUniform1i(GetUniformIndex(current, name), current->texIndex);
//    glActiveTexture(GL_TEXTURE0 + current->texIndex++);
//    glBindTexture(GL_TEXTURE_1D, Tex1D_GetHandle(value));
//    glActiveTexture(GL_TEXTURE0);
//}
//
//void PhxShader::ISetTex1D(int index, Tex1D *value) {
//    glUniform1i(index, current->texIndex);
//    glActiveTexture(GL_TEXTURE0 + current->texIndex++);
//    glBindTexture(GL_TEXTURE_1D, Tex1D_GetHandle(value));
//    glActiveTexture(GL_TEXTURE0);
//}

void PhxShader::SetTex2D(cstr name, Tex2DData *value) {
    glUniform1i(GetUniformIndex(current, name), current->texIndex);
    glActiveTexture(GL_TEXTURE0 + current->texIndex++);
    glBindTexture(GL_TEXTURE_2D, Tex2D::GetHandle(value));
    glActiveTexture(GL_TEXTURE0);
}

void PhxShader::ISetTex2D(int index, Tex2DData *value) {
    glUniform1i(index, current->texIndex);
    glActiveTexture(GL_TEXTURE0 + current->texIndex++);
    glBindTexture(GL_TEXTURE_2D, Tex2D::GetHandle(value));
    glActiveTexture(GL_TEXTURE0);
}

//void PhxShader::SetTex3D(cstr name, Tex3D *value) {
//    glUniform1i(GetUniformIndex(current, name), current->texIndex);
//    glActiveTexture(GL_TEXTURE0 + current->texIndex++);
//    glBindTexture(GL_TEXTURE_3D, Tex3D_GetHandle(value));
//    glActiveTexture(GL_TEXTURE0);
//}
//
//void PhxShader::ISetTex3D(int index, Tex3D *value) {
//    glUniform1i(index, current->texIndex);
//    glActiveTexture(GL_TEXTURE0 + current->texIndex++);
//    glBindTexture(GL_TEXTURE_3D, Tex3D_GetHandle(value));
//    glActiveTexture(GL_TEXTURE0);
//}
//
//void PhxShader::SetTexCube(cstr name, TexCube *value) {
//    glUniform1i(GetUniformIndex(current, name), current->texIndex);
//    glActiveTexture(GL_TEXTURE0 + current->texIndex++);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, TexCube_GetHandle(value));
//    glActiveTexture(GL_TEXTURE0);
//}
//
//void PhxShader::ISetTexCube(int index, TexCube *value) {
//    glUniform1i(index, current->texIndex);
//    glActiveTexture(GL_TEXTURE0 + current->texIndex++);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, TexCube_GetHandle(value));
//    glActiveTexture(GL_TEXTURE0);
//}



#define DEFAULT_CAPACITY 4

struct VarStack {
    ShaderVarType type;
    int32 size;
    int32 capacity;
    int32 elemSize;
    void *data;
};

static unordered_map<string, VarStack *> varMap {};

inline static VarStack *ShaderVar_GetStack(cstr var, ShaderVarType type) {
    VarStack *self = (VarStack *)varMap[var];
    if (!self) {
        //if (type) return 0;
        self = new VarStack();
        self->type = type;
        self->size = 0;
        self->capacity = DEFAULT_CAPACITY;
        self->elemSize = ShaderVarTypeGetSize(type);
        self->data = malloc(self->capacity * self->elemSize);
        varMap[var] = self;
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
        self->data = realloc(self->data, self->capacity * self->elemSize);
    }
    memcpy((char *)self->data + self->size * self->elemSize, value, self->elemSize);
    self->size++;
}

void PhxShaderVar::Init() {
}

void PhxShaderVar::Free() {
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
    uint handle;
    Vec2i size;
    PhxTextureFormat format;
};

inline static void Tex2D_Init() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

    self->size = Vec2i_Create(sx, sy);
    self->format = format;

    glGenTextures(1, &self->handle);
    // ToDo: There is no glActiveTexture outside glew
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glTexImage2D(GL_TEXTURE_2D, 0,
                            GetGLTextureFormat(self->format), self->size.x, self->size.y, 0,
                            IsPhxTextureFormatColor(format) ? GL_RED : GL_DEPTH_COMPONENT,
                            GL_UNSIGNED_BYTE, 0);
        Tex2D_Init();
    glBindTexture(GL_TEXTURE_2D, 0);
        return self;
}

Tex2DData *Tex2D::ScreenCapture() {
    Vec2i size; Viewport::GetSize(&size);
    Tex2DData *self = Create(size.x, size.y, PhxTextureFormat::RGBA8);
    vector<uint32_t> buffer(size.x * size.y);
    Metric::Inc(Metrics::Flush);
    glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
        for (int y = 0; y < size.y / 2; ++y)
            for (int x = 0; x < size.x; ++x)
                Swap(buffer[size.x * y + x], buffer[size.x * (size.y - y - 1) + x]);

    glBindTexture(GL_TEXTURE_2D, self->handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GetGLTextureFormat(PhxTextureFormat::RGBA8), size.x, size.y,
                            0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
        glBindTexture(GL_TEXTURE_2D, 0);
        return self;
}

void Tex2D::Acquire(Tex2DData *self) {
    //RefCounted_Acquire(self);
}

void Tex2D::Free(Tex2DData *self) {
    {
        glDeleteTextures(1, &self->handle);
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
    glBindTexture(GL_TEXTURE_2D, clone->handle);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GetGLTextureFormat(self->format), 0, 0, self->size.x, self->size.y, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    RenderTarget::Pop();
    return clone;
}

void Tex2D::Draw(Tex2DData *self, float x, float y, float sx, float sy) {
    Metric::AddDrawImm(1, 2, 4);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x, y);
    glTexCoord2f(0, 1); glVertex2f(x, y + sy);
    glTexCoord2f(1, 1); glVertex2f(x + sx, y + sy);
    glTexCoord2f(1, 0); glVertex2f(x + sx, y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Tex2D::DrawEx(Tex2DData *self, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1) {
    Metric::AddDrawImm(1, 2, 4);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glBegin(GL_QUADS);
    glTexCoord2f(u0, v0); glVertex2f(x0, y0);
    glTexCoord2f(u0, v1); glVertex2f(x0, y1);
    glTexCoord2f(u1, v1); glVertex2f(x1, y1);
    glTexCoord2f(u1, v0); glVertex2f(x1, y0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Tex2D::GenMipmap(Tex2DData *self) {
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Tex2D::GetData(Tex2DData *self, void *data, PhxPixelFormat pf, PhxDataFormat df) {
    Metric::Inc(Metrics::Flush);
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glGetTexImage(GL_TEXTURE_2D, 0, GetGLPixelFormat(pf), GetGLDataFormat(df), data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

vector<uint32_t> Tex2D::GetDataBytes(Tex2DData *self, PhxPixelFormat pf, PhxDataFormat df) {
    int size = self->size.x * self->size.y;
    size *= GetPhxDataFormatSize(df);
    size *= GetPhxPixelFormatComponents(pf);
    vector<uint32_t> data(size);
    GetData(self, data.data(), pf, df);
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
    string path = Resource::GetPath(ResourceType::Tex2D, name);
    int sx, sy, components = 4;
    uchar *data = Tex2D_LoadRaw(path.c_str(), &sx, &sy, &components);
    Tex2DData *self = Create(sx, sy, PhxTextureFormat::RGBA8);

    GLenum format =
        components == 4 ? GL_RGBA :
        components == 3 ? GL_RGB :
        components == 2 ? GL_RG : GL_RED;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glTexImage2D(GL_TEXTURE_2D, 0,
                            GL_RGBA8, self->size.x, self->size.y, 0,
                            format, GL_UNSIGNED_BYTE, data);
        Tex2D_Init();
    glBindTexture(GL_TEXTURE_2D, 0);

        delete data;
    return self;
}

void Tex2D::SetAnisotropy(Tex2DData *self, float factor) {
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, factor);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Tex2D::SetData(Tex2DData *self, void const *data, PhxPixelFormat pf, PhxDataFormat df) {
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glTexImage2D(GL_TEXTURE_2D,
                            0, GetGLTextureFormat(self->format),
                            self->size.x, self->size.y,
                            0, GetGLPixelFormat(pf), GetGLDataFormat(df), data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Tex2D::SetDataBytes(Tex2DData *self, vector<uint32_t> data, PhxPixelFormat pf, PhxDataFormat df) {
    SetData(self, data.data(), pf, df);
}

void Tex2D::SetMagFilter(Tex2DData *self, TexFilter filter) {
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Tex2D::SetMinFilter(Tex2DData *self, TexFilter filter) {
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glBindTexture(GL_TEXTURE_2D, 0);
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
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, minLevel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel);
}

void Tex2D::SetTexel(Tex2DData *self, int x, int y, float r, float g, float b, float a) {
    float rgba[] = { r, g, b, a };
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, rgba);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Tex2D::SetWrapMode(Tex2DData *self, TexWrapMode mode) {
    glBindTexture(GL_TEXTURE_2D, self->handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Tex2D::Save(Tex2DData *self, cstr path) {
    Metric::Inc(Metrics::Flush);
    glBindTexture(GL_TEXTURE_2D, self->handle);
    vector<unsigned char> buffer(4 * self->size.x * self->size.y);

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
    Tex2D_Save_Png(path, self->size.x, self->size.y, 4, buffer.data());
    glBindTexture(GL_TEXTURE_2D, 0);
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
        glEnable(GL_SCISSOR_TEST);
            float x = self->x;
        float y = self->y;
        float sx = self->sx;
        float sy = self->sy;
        TransformRect(&x, &y, &sx, &sy);
        glScissor(
            (int)x,
            vpSize.y - (int)(y + sy),
            (int)sx,
            (int)sy);
    } else {
        glDisable(GL_SCISSOR_TEST);
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
            glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
            return;
        case PhxBlendMode::Alpha:

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // There is no glBlendFuncSeparate outside glew
            //glBlendFuncSeparate(
            //    GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
            //    GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            return;
        case PhxBlendMode::PreMultAlpha:
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            return;
        case PhxBlendMode::Disabled:
            glBlendFunc(GL_ONE, GL_ZERO);
            return;
    }
}

inline static void RenderState_SetCullFace(PhxCullFace mode) {
    switch (mode) {
        case PhxCullFace::None:
            glDisable(GL_CULL_FACE);
            return;
        case PhxCullFace::Back:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            return;
        case PhxCullFace::Front:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            return;
    }
}

inline static void RenderState_SetDepthTest(bool enabled) {
    if (enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

inline static void RenderState_SetDepthWritable(bool enabled) {
    glDepthMask(enabled);
}

inline static void RenderState_SetWireframe(bool enabled) {
    if (enabled)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

    glDrawBuffers(count, bufs);
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
    Metric::Inc(Metrics::FBOSwap);
    glGenFramebuffers(1, &self->handle);
    glBindFramebuffer(GL_FRAMEBUFFER, self->handle);
    Viewport::Push(0, 0, sx, sy, false);
    FRAME_END;
}

void RenderTarget::Pop() {
    FRAME_BEGIN;
    if (fboIndex < 0)
        Fatal("RenderTarget_Pop: Attempting to pop an empty stack");

      /* Delete current FBO. */
    for (int i = 0; i < MAX_COLOR_ATTACHMENTS; ++i)
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D, 0, 0);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, 0, 0);

        glDeleteFramebuffers(1, &fboStack[fboIndex].handle);
        fboIndex--;

        /* Activate previous FBO. */
    Metric::Inc(Metrics::FBOSwap);
    if (fboIndex >= 0)
        glBindFramebuffer(GL_FRAMEBUFFER, GetActive()->handle);
    else
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + self->colorIndex++,
            GL_TEXTURE_2D, handle, level);
            SetDrawBuffers(self->colorIndex);
    }

    /* Depth attachment. */
    else {
        if (self->depth)
            Fatal("RenderTarget_BindTex2D: Target already has a depth buffer");
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, handle, level);
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
//    glFramebufferTexture3D(
//        GL_FRAMEBUFFER,
//        GL_COLOR_ATTACHMENT0 + self->colorIndex++,
//        GL_TEXTURE_3D, handle, level, layer);
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
//    glFramebufferTexture2D(
//        GL_FRAMEBUFFER,
//        GL_COLOR_ATTACHMENT0 + self->colorIndex++,
//        face, handle, level);
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
    glColor4f(color.x, color.y, color.z, color.w * alpha);
}

void Draw::PopAlpha() {
    if (alphaIndex < 0)
        Fatal("Draw_PopAlpha Attempting to pop an empty alpha stack");

    alphaIndex--;
    float alpha = alphaIndex >= 0 ? alphaStack[alphaIndex] : 1;
    glColor4f(color.x, color.y, color.z, color.w * alpha);
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
    glEnd();

    glBegin(GL_POINTS);
    glColor4f(1, 1, 1, _alpha);
    glVertex3f(UNPACK3(*pos));
    glEnd();
}

void Draw::Border(float s, float x, float y, float w, float h) {
    Draw::Rect(x, y, w, s);
    Draw::Rect(x, y + h - s, w, s);
    Draw::Rect(x, y + s, s, h - 2 * s);
    Draw::Rect(x + w - s, y + s, s, h - 2 * s);
}

void Draw::Box3(Box3f const *self) {
    Metric::AddDrawImm(6, 12, 24);
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
    glEnd();
}

void Draw::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Draw::ClearDepth(float d) {
    glClearDepth(d);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Draw::Color(float r, float g, float b, float a) {
    float alpha = alphaIndex >= 0 ? alphaStack[alphaIndex] : 1;
    color = Vec4f_Create(r, g, b, a);
    glColor4f(r, g, b, a * alpha);
}

void Draw::Flush() {
    Metric::Inc(Metrics::Flush);
    glFinish();
}

void Draw::Line(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void Draw::Line3(Vec3f const *p1, Vec3f const *p2) {
    glBegin(GL_LINES);
    glVertex3f(UNPACK3(*p1));
    glVertex3f(UNPACK3(*p2));
    glEnd();
}

void Draw::LineWidth(float width) {
    glLineWidth(width);
}

void Draw::Plane(Vec3f const *p, Vec3f const *n, float scale) {
    Vec3f e1 = Abs(n->x) < 0.7f ? Vec3f_Create(1, 0, 0) : Vec3f_Create(0, 1, 0);
    e1 = Vec3f_Normalize(Vec3f_Reject(e1, *n));
    Vec3f e2 = Vec3f_Cross(*n, e1);

    Vec3f p0 = Vec3f_Add(*p, Vec3f_Add(Vec3f_Muls(e1, -scale), Vec3f_Muls(e2, -scale)));
    Vec3f p1 = Vec3f_Add(*p, Vec3f_Add(Vec3f_Muls(e1, scale), Vec3f_Muls(e2, -scale)));
    Vec3f p2 = Vec3f_Add(*p, Vec3f_Add(Vec3f_Muls(e1, scale), Vec3f_Muls(e2, scale)));
    Vec3f p3 = Vec3f_Add(*p, Vec3f_Add(Vec3f_Muls(e1, -scale), Vec3f_Muls(e2, scale)));

    Metric::AddDrawImm(1, 2, 4);
    glBegin(GL_QUADS);
    glVertex3f(UNPACK3(p0));
    glVertex3f(UNPACK3(p1));
    glVertex3f(UNPACK3(p2));
    glVertex3f(UNPACK3(p3));
    glEnd();
}

void Draw::Point(float x, float y) {
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void Draw::Point3(float x, float y, float z) {
    glBegin(GL_POINTS);
    glVertex3f(x, y, z);
    glEnd();
}

void Draw::PointSize(float size) {
    glPointSize(size);
}

void Draw::Poly(Vec2f const *points, int count) {
    Metric::AddDrawImm(1, count - 2, count);
    glBegin(GL_POLYGON);
    for (int i = 0; i < count; ++i)
        glVertex2f(UNPACK2(points[i]));
    glEnd();
}

void Draw::Poly3(Vec3f const *points, int count) {
    Metric::AddDrawImm(1, count - 2, count);
    glBegin(GL_POLYGON);
    for (int i = 0; i < count; ++i)
        glVertex3f(UNPACK3(points[i]));
    glEnd();
}

void Draw::Quad(Vec2f const *p1, Vec2f const *p2, Vec2f const *p3, Vec2f const *p4) {
    Metric::AddDrawImm(1, 2, 4);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(UNPACK2(*p1));
    glTexCoord2f(0, 1); glVertex2f(UNPACK2(*p2));
    glTexCoord2f(1, 1); glVertex2f(UNPACK2(*p3));
    glTexCoord2f(1, 0); glVertex2f(UNPACK2(*p4));
    glEnd();
}

void Draw::Quad3(Vec3f const *p1, Vec3f const *p2, Vec3f const *p3, Vec3f const *p4) {
    Metric::AddDrawImm(1, 2, 4);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(UNPACK3(*p1));
    glTexCoord2f(0, 1); glVertex3f(UNPACK3(*p2));
    glTexCoord2f(1, 1); glVertex3f(UNPACK3(*p3));
    glTexCoord2f(1, 0); glVertex3f(UNPACK3(*p4));
    glEnd();
}

void Draw::Rect(float x1, float y1, float xs, float ys) {
    float x2 = x1 + xs;
    float y2 = y1 + ys;
    Metric::AddDrawImm(1, 2, 4);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x1, y1);
    glTexCoord2f(0, 1); glVertex2f(x1, y2);
    glTexCoord2f(1, 1); glVertex2f(x2, y2);
    glTexCoord2f(1, 0); glVertex2f(x2, y1);
    glEnd();
}

void Draw::SmoothLines(bool enabled) {
    if (enabled) {
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    }
}

void Draw::SmoothPoints(bool enabled) {
    if (enabled) {
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    } else {
        glDisable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
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
        Metric::AddDrawImm(res, res, res * 3);
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
        glEnd();
    }

    /* Middle Rows */ {
        Metric::AddDrawImm(res - 2, 2 * (res - 2), 4 * (res - 2));
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
        glEnd();
    }

    /* Bottom Row */ {
        Metric::AddDrawImm(res, res, res * 3);
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
        glEnd();
    }
}

void Draw::Tri(Vec2f const *v1, Vec2f const *v2, Vec2f const *v3) {
    Metric::AddDrawImm(1, 1, 3);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0); glVertex2f(UNPACK2(*v1));
    glTexCoord2f(0, 1); glVertex2f(UNPACK2(*v2));
    glTexCoord2f(1, 1); glVertex2f(UNPACK2(*v3));
    glEnd();
}

void Draw::Tri3(Vec3f const *v1, Vec3f const *v2, Vec3f const *v3) {
    Metric::AddDrawImm(1, 1, 3);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0); glVertex3f(UNPACK3(*v1));
    glTexCoord2f(0, 1); glVertex3f(UNPACK3(*v2));
    glTexCoord2f(1, 1); glVertex3f(UNPACK3(*v3));
    glEnd();
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
    glViewport(self->x, self->y, self->sx, self->sy);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* GL's window coordinates and texture coordinates have opposite vertical
        * orientation. Automatically compensate via the projection matrix. */
    if (self->isWindow) {
        glTranslatef(-1.0, 1.0, 0.0);
        glScalef(2.0f / self->sx, -2.0f / self->sy, 1.0f);
    } else {
        glTranslatef(-1.0, -1.0, 0.0);
        glScalef(2.0f / self->sx, 2.0f / self->sy, 1.0f);
    }
}

float Viewport::GetAspect() {
    if (vpIndex < 0)
        Fatal("Viewport_GetAspect: Viewport stack is empty");
    return (float)vp[vpIndex].sx / (float)vp[vpIndex].sy;
}

void Viewport::GetSize(Vec2i *out) {
    if (vpIndex < 0) Fatal("Viewport_GetSize: Viewport stack is empty");
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
