module;

#include "Box3.h"
#include "Vec2.h"

export module Ultra.Engine.Phoenix;

import Ultra.Core;

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

enum class ResourceType {
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

    Count   = 0xA,
};

class Resource {
public:
    static void    AddPath(ResourceType, cstr format);
    static bool    Exists(ResourceType, cstr name);
    static cstr    GetPath(ResourceType, cstr name);
    static Bytes  *LoadBytes(ResourceType, cstr name);
    static cstr    LoadCstr(ResourceType, cstr name);

    static cstr    ToString(ResourceType);
    static void    Init();
};

}

///
/// @brief Shader
///
export namespace Ultra {

enum class  ShaderVarType {
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

struct ShaderData;
struct Tex2DData;

class PhxShader {
public:
    static ShaderData *Create(cstr vertCode, cstr fragCode);
    static ShaderData *Load(cstr vertName, cstr fragName);
    static void          Acquire(ShaderData *);
    static void          Free(ShaderData *);
    static ShaderState   *ToShaderState(ShaderData *);
    
    static void          Start(ShaderData *);
    static void          Stop(ShaderData *);
    
    static uint          GetHandle(ShaderData *);
    static int           GetVariable(ShaderData *, cstr);
    static bool          HasVariable(ShaderData *, cstr);
    
    static void          ClearCache();
    static void          SetFloat(cstr, float);
    static void          SetFloat2(cstr, float, float);
    static void          SetFloat3(cstr, float, float, float);
    static void          SetFloat4(cstr, float, float, float, float);
    static void          SetInt(cstr, int);
    static void          SetMatrix(cstr, Matrix *);
    static void          SetMatrixT(cstr, Matrix *);
    //static void          SetTex1D(cstr, Tex1D *);
    static void          SetTex2D(cstr, Tex2DData *);
    //static void          SetTex3D(cstr, Tex3D *);
    static void          SetTexCube(cstr, TexCube *);
    
    static void          ISetFloat(int, float);
    static void          ISetFloat2(int, float, float);
    static void          ISetFloat3(int, float, float, float);
    static void          ISetFloat4(int, float, float, float, float);
    static void          ISetInt(int, int);
    static void          ISetMatrix(int, Matrix *);
    static void          ISetMatrixT(int, Matrix *);
    //static void          ISetTex1D(int, Tex1D *);
    static void          ISetTex2D(int, Tex2DData *);
    //static void          ISetTex3D(int, Tex3D *);
    //static void          ISetTexCube(int, TexCube *);
};

class PhxShaderVar {
    public:
    static void *Get(cstr, ShaderVarType);
    static void  PushFloat(cstr, float);
    static void  PushFloat2(cstr, float, float);
    static void  PushFloat3(cstr, float, float, float);
    static void  PushFloat4(cstr, float, float, float, float);
    static void  PushInt(cstr, int);
    static void  PushMatrix(cstr, Matrix *);
    static void  PushTex1D(cstr, Tex1D *);
    static void  PushTex2D(cstr, Tex2DData *);
    static void  PushTex3D(cstr, Tex3D *);
    static void  PushTexCube(cstr, TexCube *);
    static void  Pop(cstr);

    static void Init();
    static void Free();
};

}

///
/// @brief Texture2D
///
export namespace Ultra {

struct Tex2DData;

class Tex2D {
public:
    static Tex2DData *Create(int sx, int sy, PhxTextureFormat);
    static Tex2DData *Load(cstr name);
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
    static Bytes      *GetDataBytes(Tex2DData *, PhxPixelFormat, PhxDataFormat);
    static PhxTextureFormat  GetFormat(Tex2DData *);
    static uint       GetHandle(Tex2DData *);
    static void       GetSize(Tex2DData *, Vec2i *out);
    static void       GetSizeLevel(Tex2DData *, Vec2i *out, int level);
    static void       SetAnisotropy(Tex2DData *, float);
    static void       SetData(Tex2DData *, void const *, PhxPixelFormat, PhxDataFormat);
    static void       SetDataBytes(Tex2DData *, Bytes *, PhxPixelFormat, PhxDataFormat);
    static void       SetMagFilter(Tex2DData *, TexFilter);
    static void       SetMinFilter(Tex2DData *, TexFilter);
    static void       SetMipRange(Tex2DData *, int minLevel, int maxLevel);
    static void       SetTexel(Tex2DData *, int x, int y, float r, float g, float b, float a);
    static void       SetWrapMode(Tex2DData *, TexWrapMode);

    static void       Save(Tex2DData *, cstr path);
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
    static void Poly(Vec2f const *points, int count);
    static void Quad(Vec2f const *p1, Vec2f const *p2, Vec2f const *p3, Vec2f const *p4);
    static void Rect(float x, float y, float sx, float sy);
    static void Tri(Vec2f const *p1, Vec2f const *p2, Vec2f const *p3);

    /* --- 3D API --------------------------------------------------------------- */

    static void Axes(Vec3f const *pos, Vec3f const *x, Vec3f const *y, Vec3f const *z, float scale, float alpha);
    static void Box3(Box3f const *box);
    static void Line3(Vec3f const *p1, Vec3f const *p2);
    static void Plane(Vec3f const *p, Vec3f const *n, float scale);
    static void Point3(float x, float y, float z);
    static void Poly3(Vec3f const *points, int count);
    static void Quad3(Vec3f const *p1, Vec3f const *p2, Vec3f const *p3, Vec3f const *p4);
    static void Sphere(Vec3f const *p, float r);
    static void Tri3(Vec3f const *p1, Vec3f const *p2, Vec3f const *p3);
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
    static void   GetSize(Vec2i *out);
};

}

///
/// @brief Window
///
export namespace Ultra {

struct WindowData;

enum class WindowMode {
    AlwaysOnTop = 32768,
    Borderless  = 16,
    Fullscreen  = 4097,
    Hidden      = 8,
    Maximized   = 128,
    Minimized   = 64,
    Resizable   = 32,
    Shown       = 4,
};

enum class PhxWindowPosition {
    Centered    = 0x2FFF0000u,
    Default     = 0x1FFF0000u,
};

class PhxWindow {
public:
    static WindowData *Create(cstr title, PhxWindowPosition x, PhxWindowPosition y, int sx, int sy, WindowMode mode);
    static void     Free(WindowData *);
    
    static void     BeginDraw(WindowData *);
    static void     EndDraw(WindowData *);
    
    static void     GetPosition(WindowData *, Vec2i *out);
    static void     GetSize(WindowData *, Vec2i *out);
    static cstr     GetTitle(WindowData *);
    
    static void     SetFullscreen(WindowData *, bool);
    static void     SetPosition(WindowData *, PhxWindowPosition, PhxWindowPosition);
    static void     SetSize(WindowData *, int, int);
    static void     SetTitle(WindowData *, cstr);
    static void     SetVsync(WindowData *, bool);
    
    static void     ToggleFullscreen(WindowData *);
    
    static void     Hide(WindowData *);
    static void     Show(WindowData *);
};

}
