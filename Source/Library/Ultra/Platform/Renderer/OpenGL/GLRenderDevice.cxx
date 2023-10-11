module;

#include <glad/gl.h>

module Ultra.Platform.Renderer.GLRenderDevice;

import Ultra.Renderer;

#pragma warning(push)
#pragma warning(disable: 4100)

import <stacktrace>;

namespace Ultra {

// Helpers
static void GLMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    auto &&stacktrace = ((std::stacktrace *)userParam)->current();
    string line {};
    if (stacktrace.size() >= 2) {
        auto lastInternalEntry = std::ranges::find_if(stacktrace | std::views::drop(1), [](const auto &entry) {
            auto &&view = entry.description();
            return view.find("Ultra") != string_view::npos;
        });

        if (lastInternalEntry != stacktrace.end()) {
            line = std::format("{}[Line: {}]", lastInternalEntry->source_file(), lastInternalEntry->source_line());
        } else {
            line = "StackTrace: There are no internal entries!";
        }
    } else {
        line = "StackTrace: Empty!";
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            LogError("Platform::OpenGL: {}\n  @{}", message, line);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            LogWarning("Platform::OpenGL: {}\n  @{}", message, line);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            LogInfo("Platform::OpenGL: {}\n  @{}", message, line);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            LogTrace("Platform::OpenGL: {}\n  @{}", message, line);
            break;
        case GL_DONT_CARE:
            Log("Platform::OpenGL: {}\n  @{}", message, line);
            break;
    }
}


GLRenderDevice::GLRenderDevice() {}

GLRenderDevice::~GLRenderDevice() {}

void GLRenderDevice::Load() {
    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Debugging-Support
    std::stacktrace trace;
    glDebugMessageCallback(GLMessage, &trace);
    //glDebugMessageCallback(GLMessage, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    // 3D-Support
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);   // Interprets a smaller value as "closer"

    // Anti-Aliasing Support
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

    // Clipping-Support (triangles drawn anti-clock-wize are the front face)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Color-Mixing and Transparency-Support
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Pixel Byte Pack/Unpack-Alignment
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Multisampling-Support
    glEnable(GL_MULTISAMPLE);

    // Information
    auto &capabilities = RenderDevice::GetCapabilities();
    capabilities.Vendor = (const char *)glGetString(GL_VENDOR);
    capabilities.Model = (const char *)glGetString(GL_RENDERER);
    capabilities.Version = (const char *)glGetString(GL_VERSION);
    capabilities.SLVersion = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

    // ToDo: Crawl for more information
    int nrAttributes {};
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

    int extensionCount = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
    capabilities.Extensions.reserve(extensionCount);
    for (int i = 0; i < extensionCount; i++) {
        capabilities.Extensions.push_back((const char *)glGetStringi(GL_EXTENSIONS, i));
    }

    // Limits
    glGetIntegerv(GL_MAX_SAMPLES, &capabilities.MaxSamples);
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &capabilities.MaxAnisotropy);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &capabilities.MaxTextureUnits);

    // Uniform Block Size
    GLint maxUniformBlockSize;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
    printf("Max uniform block size: %d\n", maxUniformBlockSize);

    glBindVertexArray(0);
    capabilities.Log();
};

void GLRenderDevice::BeginFrame() {};

void GLRenderDevice::EndFrame() {};

void GLRenderDevice::Dispose() {};


void GLRenderDevice::SetLineThickness(float value) {
    GLfloat range[2];
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, range);
    // ToDo: Fails while closing application, the Update function should somehow finish cleanly.
    //APP_ASSERT(!(value < range[0] || value > range[1]), "The specified line width exceeds the supported range!")
    glLineWidth(value);
}

void GLRenderDevice::SetPolygonMode(PolygonMode mode) {
    switch (mode) {
        case PolygonMode::Solid:     { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break; }
        case PolygonMode::Wireframe: { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break; }
        default: {
            //AppAssert(false, "The specified polygon mode isn't implemented yet!");
            break;
        }
    }
}

}

#pragma warning(pop)
