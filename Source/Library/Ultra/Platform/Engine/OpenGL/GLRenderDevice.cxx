module;

#include <glad/gl.h>

module Ultra.Platform.Renderer.GLRenderDevice;

import Ultra.Engine.Renderer;

namespace Ultra {

// Helpers
static void GLMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            LogError("Platform::OpenGL: ", message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            LogWarning("Platform::OpenGL: ", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            LogInfo("Platform::OpenGL: ", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            LogTrace("Platform::OpenGL: ", message);
            break;
    }
}


GLRenderDevice::GLRenderDevice() {}

GLRenderDevice::~GLRenderDevice() {}

void GLRenderDevice::Load() {
    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Properties
    glDebugMessageCallback(GLMessage, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    //glFrontFace(GL_CCW);

    //glEnable(GL_MULTISAMPLE);	// Information
    //glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    // Information
    auto &capabilities = RenderDevice::GetCapabilities();
    capabilities.Vendor = (const char *)glGetString(GL_VENDOR);
    capabilities.Model = (const char *)glGetString(GL_RENDERER);
    capabilities.Version = (const char *)glGetString(GL_VERSION);
    capabilities.SLVersion = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

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
        case PolygonMode::Solid: { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break; }
        case PolygonMode::Wireframe: { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break; }
        default: {
            AppAssert(false, "The specified polygon mode isn't implemented yet!");
            break;
        }
    }
}

}
