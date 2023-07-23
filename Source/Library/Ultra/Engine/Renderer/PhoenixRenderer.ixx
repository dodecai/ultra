export module Ultra.Engine.Phoenix;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;

///
/// @brief Data
///
export namespace Ultra {

enum class PhxBlendMode: int32_t {
    Additive = 0,
    Alpha = 1,
    Disabled = 2,
    PreMultAlpha = 3,
};

enum class PhxCullFace: int32_t {
    None    = 0,
    Back    = 1,
    Front   = 2,
};

enum class PhxDataFormat: int32_t {
    U8,
    I8,
    U16,
    I16,
    U32,
    I32,
    Float,
};

int GetPhxDataFormatSize(PhxDataFormat self) {
    switch (self) {
        case PhxDataFormat::U8:
        case PhxDataFormat::I8:
            return 1;
        case PhxDataFormat::U16:
        case PhxDataFormat::I16:
            return 2;
        case PhxDataFormat::U32:
        case PhxDataFormat::I32:
        case PhxDataFormat::Float:
            return 4;
    }
    return 0;
}

enum class PhxPixelFormat: int32_t {
    Red,
    RG,
    RGB,
    BGR,
    RGBA,
    BGRA,
    DepthComponent,
};

int GetPhxPixelFormatComponents(PhxPixelFormat self) {
    switch (self) {
        case PhxPixelFormat::Red:
        case PhxPixelFormat::DepthComponent:
            return 1;
        case PhxPixelFormat::RG:
            return 2;
        case PhxPixelFormat::RGB:
        case PhxPixelFormat::BGR:
            return 3;
        case PhxPixelFormat::RGBA:
        case PhxPixelFormat::BGRA:
            return 4;
    }
    return 0;
}

enum class PhxTextureFormat: int32_t {
    R8,
    R16,
    R16F,
    R32F,
    RG8,
    RG16,
    RG16F,
    RG32F,
    RGB8,
    RGBA8,
    RGBA16,
    RGBA16F,
    RGBA32F,

    Depth16,
    Depth24,
    Depth32F,
};

int GetPhxTextureFormatComponents(PhxTextureFormat self) {
    switch (self) {
        case PhxTextureFormat::R8:
        case PhxTextureFormat::R16:
        case PhxTextureFormat::R16F:
        case PhxTextureFormat::R32F:
        case PhxTextureFormat::Depth16:
        case PhxTextureFormat::Depth24:
        case PhxTextureFormat::Depth32F:
            return 1;
        case PhxTextureFormat::RG8:
        case PhxTextureFormat::RG16:
        case PhxTextureFormat::RG16F:
        case PhxTextureFormat::RG32F:
            return 2;
        case PhxTextureFormat::RGB8:
            return 3;
        case PhxTextureFormat::RGBA8:
        case PhxTextureFormat::RGBA16:
        case PhxTextureFormat::RGBA16F:
        case PhxTextureFormat::RGBA32F:
            return 4;
    }
    return 0;
}

int GetPhxTextureFormatSize(PhxTextureFormat self) {
    switch (self) {
        case PhxTextureFormat::R8:
            return 1;
        case PhxTextureFormat::R16:
        case PhxTextureFormat::R16F:
        case PhxTextureFormat::RG8:
        case PhxTextureFormat::Depth16:
            return 2;
        case PhxTextureFormat::RGB8:
        case PhxTextureFormat::Depth24:
            return 3;
        case PhxTextureFormat::R32F:
        case PhxTextureFormat::RG16:
        case PhxTextureFormat::RG16F:
        case PhxTextureFormat::RGBA8:
        case PhxTextureFormat::Depth32F:
            return 4;
        case PhxTextureFormat::RG32F:
        case PhxTextureFormat::RGBA16:
        case PhxTextureFormat::RGBA16F:
            return 8;
        case PhxTextureFormat::RGBA32F:
            return 16;
    }
    return 0;
}

bool IsPhxTextureFormatColor(PhxTextureFormat self) {
    switch (self) {
        case PhxTextureFormat::Depth16:
        case PhxTextureFormat::Depth24:
        case PhxTextureFormat::Depth32F:
            return false;
    }
    return true;
}

bool IsPhxTextureFormatDepth(PhxTextureFormat self) {
    switch (self) {
        case PhxTextureFormat::Depth16:
        case PhxTextureFormat::Depth24:
        case PhxTextureFormat::Depth32F:
            return true;
    }
    return false;
}

bool IsPhxTextureFormatValid(PhxTextureFormat self) {
    switch (self) {
        case PhxTextureFormat::R8:
        case PhxTextureFormat::R16:
        case PhxTextureFormat::R16F:
        case PhxTextureFormat::R32F:
        case PhxTextureFormat::RG8:
        case PhxTextureFormat::RG16:
        case PhxTextureFormat::RG16F:
        case PhxTextureFormat::RG32F:
        case PhxTextureFormat::RGB8:
        case PhxTextureFormat::RGBA8:
        case PhxTextureFormat::RGBA16:
        case PhxTextureFormat::RGBA16F:
        case PhxTextureFormat::RGBA32F:
        case PhxTextureFormat::Depth16:
        case PhxTextureFormat::Depth24:
        case PhxTextureFormat::Depth32F:
            return true;
    }
    return false;
}

}

///
/// @brief Resource
///
export namespace Ultra {

enum class PhyResourceType {
    Font    = 0,
    Mesh    = 1,
    Other   = 2,
    Script  = 3,
    Shader  = 4,
    Sound   = 5,
    Tex1D   = 6,
    Tex2D   = 7,
    Tex3D   = 8,
    TexCube = 9,
};

class Resource {

public:
    Resource();
    ~Resource();

    static const Resource &Instance() {
        static Resource instance {};
        return instance;
    }

    static bool Exists(PhyResourceType, const string &name);
    static string GetPath(PhyResourceType, const string &name);
    static vector<uint32_t> LoadBytes(PhyResourceType, const string &name);
    static string LoadCstr(PhyResourceType, const string &name);
    static void Reset();

private:
    static void AddPath(PhyResourceType, const string &format);
    static string Resolve(PhyResourceType type, const string &name);
    static string ToString(PhyResourceType);
};

}

///
/// @brief Metrics
///
export namespace Ultra {

/* --- API NOTES ---------------------------------------------------------------
 *
 *   The Metric API is a simple set of accumulators that engine functions may
 *   call to provide information relevent to performance profiling and
 *   debugging. Metrics are reset each frame in Engine_Update, hence provide
 *   only instantaneous information.
 *
 *   Metrics are quantities that only code internal to the engine can change
 *   (e.g., a Lua script cannot draw a polygon without passing through
 *   Draw_*, Mesh_Draw*, etc). Hence, all functions that modify metrics are
 *   private.
 *
 *   Metric_Reset is called automatically in Engine_Update, so that metrics
 *   are per-frame.
 *
 *     DrawCalls  : # gl_Draw* calls. glBegin/glEnd does not count.
 *     Immediate  : # glBegin/glEnd pairs.
 *     PolysDrawn : # polys drawn. gl_Begin(GL_QUADS) et al. should report the
 *                  number of *polygons*, not triangles!
 *
 *     TrisDrawn  : # tris drawn. Non-tri draw calls should report
 *                  #verts - 2, e.g. number of tris required for convex
 *                  decomposition.
 *
 *     VertsDrawn : # verts *referenced* in draw calls. Buffered draws could
 *                  report only the size of the vertex buffer! Immediate
 *                  mode should report # calls to glVertex*.
 *
 *     Flush      : An *upper bound* on pipeline flushes. Each call that can
 *                  cause a flush should increment this metric.
 *
 *     FBOSwap    : # glBindFramebuffer calls. Each call should be reported.
 *                  In the future, we will introduce a new metric to report #
 *                  of validation-inducing bind calls, but this will only be
 *                  relevant once fbo caching is implemented.
 *
 * -------------------------------------------------------------------------- */

enum class Metrics {
    None = 0x0,
    DrawCalls = 0x1,
    Immediate = 0x2,
    PolysDrawn = 0x3,
    TrisDrawn = 0x4,
    VertsDrawn = 0x5,
    Flush = 0x6,
    FBOSwap = 0x7,
    SIZE = 0x7,
};

class PhxMetric {
public:
    static int32_t  Get(Metrics);
    static const char *   GetName(Metrics);

    static void   Inc(Metrics);
    static void   Mod(Metrics, int32_t);
    static void   Reset();
    
    static void   AddDraw(int32_t polys, int32_t tris, int32_t verts);
    static void   AddDrawImm(int32_t polys, int32_t tris, int32_t verts);
};

}

///
/// @brief Shader
///
export namespace Ultra {

enum class  PhxShaderVarType {
    None = 0x0,
    BEGIN = 0x1,
    Float = 0x1,
    Float2 = 0x2,
    Float3 = 0x3,
    Float4 = 0x4,
    Int = 0x5,
    Int2 = 0x6,
    Int3 = 0x7,
    Int4 = 0x8,
    Matrix = 0x9,
    Tex1D = 0xA,
    Tex2D = 0xB,
    Tex3D = 0xC,
    TexCube = 0xD,
    END = 0xD,
    SIZE = 0xD,
};

/* --- API NOTES ---------------------------------------------------------------
 *
 *   This type is REFERENCE-COUNTED. See ../doc/RefCounted.txt for details.
 *
 * -------------------------------------------------------------------------- */

struct ShaderState;
struct ShaderData;
struct Tex2DData;

class PhxShader {
public:
    static ShaderData *Create(const char * vertCode, const char * fragCode);
    static ShaderData *Load(const char * vertName, const char * fragName);
    static void          Acquire(ShaderData *);
    static void          Free(ShaderData *);
    static ShaderState   *ToShaderState(ShaderData *);
    
    static void          Start(ShaderData *);
    static void          Stop(ShaderData *);
    
    static uint32_t          GetHandle(ShaderData *);
    static int           GetVariable(ShaderData *, const char *);
    static bool          HasVariable(ShaderData *, const char *);
    
    static void          ClearCache();
    static void          SetFloat(const char *, float);
    static void          SetFloat2(const char *, float, float);
    static void          SetFloat3(const char *, float, float, float);
    static void          SetFloat4(const char *, float, float, float, float);
    static void          SetInt(const char *, int);
    static void          SetMatrix(const char *, NMatrix4f *);
    static void          SetMatrixT(const char *, NMatrix4f *);
    //static void          SetTex1D(const char *, Tex1D *);
    static void          SetTex2D(const char *, Tex2DData *);
    //static void          SetTex3D(const char *, Tex3D *);
    //static void          SetTexCube(const char *, TexCube *);
    
    static void          ISetFloat(int, float);
    static void          ISetFloat2(int, float, float);
    static void          ISetFloat3(int, float, float, float);
    static void          ISetFloat4(int, float, float, float, float);
    static void          ISetInt(int, int);
    static void          ISetMatrix(int, NMatrix4f *);
    static void          ISetMatrixT(int, NMatrix4f *);
    //static void          ISetTex1D(int, Tex1D *);
    static void          ISetTex2D(int, Tex2DData *);
    //static void          ISetTex3D(int, Tex3D *);
    //static void          ISetTexCube(int, TexCube *);
};

class PhxShaderVar {
    public:
    static void *Get(const char *, PhxShaderVarType);
    static void  PushFloat(const char *, float);
    static void  PushFloat2(const char *, float, float);
    static void  PushFloat3(const char *, float, float, float);
    static void  PushFloat4(const char *, float, float, float, float);
    static void  PushInt(const char *, int);
    static void  PushMatrix(const char *, NMatrix4f *);
    //static void  PushTex1D(const char *, Tex1D *);
    static void  PushTex2D(const char *, Tex2DData *);
    //static void  PushTex3D(const char *, Tex3D *);
    //static void  PushTexCube(const char *, TexCube *);
    static void  Pop(const char *);

    static void Init();
    static void Free();
};

}

///
/// @brief Texture2D
///
export namespace Ultra {

struct Tex2DData;

enum class PhxTexFilter {
    Point,
    PointMipPoint,
    PointMipLinear,
    Linear,
    LinearMipPoint,
    LinearMipLinear,
};

enum class PhxTexWrapMode {
    Clamp,
    MirrorClamp,
    MirrorRepeat,
    Repeat,
};


class Tex2D {
public:
    static Tex2DData *Create(int sx, int sy, PhxTextureFormat);
    static Tex2DData *Load(const char * name);
    static Tex2DData *ScreenCapture();
    static void       Acquire(Tex2DData *);
    static void       Free(Tex2DData *);

    static void       Pop(Tex2DData *);
    static void       Push(Tex2DData *);
    static void       PushLevel(Tex2DData *, int level);

    static void       Clear(Tex2DData *, float r, float g, float b, float a);
    static Tex2DData *Clone(Tex2DData *);
    static void       Draw(Tex2DData *, float x, float y, float sx, float sy);
    static void       DrawEx(Tex2DData *, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1);
    static void       GenMipmap(Tex2DData *);
    static void       GetData(Tex2DData *, void *, PhxPixelFormat, PhxDataFormat);
    static vector<uint32_t>      GetDataBytes(Tex2DData *, PhxPixelFormat, PhxDataFormat);
    static PhxTextureFormat  GetFormat(Tex2DData *);
    static uint32_t       GetHandle(Tex2DData *);
    static void       GetSize(Tex2DData *, Vector2Di *out);
    static void       GetSizeLevel(Tex2DData *, Vector2Di *out, int level);
    static void       SetAnisotropy(Tex2DData *, float);
    static void       SetData(Tex2DData *, void const *, PhxPixelFormat, PhxDataFormat);
    static void       SetDataBytes(Tex2DData *, vector<uint32_t>, PhxPixelFormat, PhxDataFormat);
    static void       SetMagFilter(Tex2DData *, PhxTexFilter);
    static void       SetMinFilter(Tex2DData *, PhxTexFilter);
    static void       SetMipRange(Tex2DData *, int minLevel, int maxLevel);
    static void       SetTexel(Tex2DData *, int x, int y, float r, float g, float b, float a);
    static void       SetWrapMode(Tex2DData *, PhxTexWrapMode);

    static void       Save(Tex2DData *, const char * path);
};

}

///
/// @brief ClipRect
///
export namespace Ultra {

class ClipRect {
    public:
    static void Push(float x, float y, float sx, float sy);
    static void PushCombined(float x, float y, float sx, float sy);
    static void PushDisabled();
    static void PushTransform(float tx, float ty, float sx, float sy);
    static void Pop();
    static void PopTransform();
};

}

///
/// @brief RenderState
///
export namespace Ultra {

class RenderState {
public:
    static void  PushAllDefaults();
    static void  PushBlendMode(PhxBlendMode mode);
    static void  PushCullFace(PhxCullFace face);
    static void  PushDepthTest(bool);
    static void  PushDepthWritable(bool);
    static void  PushWireframe(bool);
    static void  PopAll();
    static void  PopBlendMode();
    static void  PopCullFace();
    static void  PopDepthTest();
    static void  PopDepthWritable();
    static void  PopWireframe();
};

}

///
/// @brief RenderTarget
///
export namespace Ultra {

class RenderTarget {
public:
    static void  Push(int sx, int sy);
    static void  Pop();

    static void  BindTex2D(Tex2DData *);
    static void  BindTex2DLevel(Tex2DData *, int level);
    //static void  BindTex3D(Tex3D *, int layer);
    //static void  BindTex3DLevel(Tex3D *, int layer, int level);
    //static void  BindTexCube(TexCube *, CubeFace);
    //static void  BindTexCubeLevel(TexCube *, CubeFace, int level);

    static void  PushTex2D(Tex2DData *);
    static void  PushTex2DLevel(Tex2DData *, int level);
    //static void  PushTex3D(Tex3D *, int layer);
    //static void  PushTex3DLevel(Tex3D *, int layer, int level);
};


}

///
/// @brief Draw
///
export namespace Ultra {

class Draw {
public:
    static void Clear(float r, float g, float b, float a);
    static void ClearDepth(float d);
    static void Color(float r, float g, float b, float a);
    static void Flush();
    static void LineWidth(float width);
    static void PointSize(float size);
    static void PushAlpha(float a);
    static void PopAlpha();
    static void SmoothLines(bool);
    static void SmoothPoints(bool);

    /* --- 2D API --------------------------------------------------------------- */

    static void Border(float s, float x, float y, float w, float h);
    static void Line(float x1, float y1, float x2, float y2);
    static void Point(float x, float y);
    static void Poly(Vector2Df const *points, int count);
    static void Quad(Vector2Df const *p1, Vector2Df const *p2, Vector2Df const *p3, Vector2Df const *p4);
    static void Rect(float x, float y, float sx, float sy);
    static void Tri(Vector2Df const *p1, Vector2Df const *p2, Vector2Df const *p3);

    /* --- 3D API --------------------------------------------------------------- */

    static void Axes(Vector3Df const *pos, Vector3Df const *x, Vector3Df const *y, Vector3Df const *z, float scale, float alpha);
    /*static void Box3(Box3f const *box);*/
    static void Line3(Vector3Df const *p1, Vector3Df const *p2);
    static void Plane(Vector3Df const *p, Vector3Df const *n, float scale);
    static void Point3(float x, float y, float z);
    static void Poly3(Vector3Df const *points, int count);
    static void Quad3(Vector3Df const *p1, Vector3Df const *p2, Vector3Df const *p3, Vector3Df const *p4);
    static void Sphere(Vector3Df const *p, float r);
    static void Tri3(Vector3Df const *p1, Vector3Df const *p2, Vector3Df const *p3);
};

}

///
/// @brief Viewport
///
export namespace Ultra {

class Viewport {
public:
    static void   Pop();
    static void   Push(int x, int y, int sx, int sy, bool isWindow);
    static float  GetAspect();
    static void   GetSize(Vector2Di *out);
};

}
