module Ultra.Engine.Renderer;

import Ultra.Platform.DXRenderer;
import Ultra.Platform.GLRenderer;
import Ultra.Platform.VKRenderer;
import Ultra.Platform.Renderer.DXRenderDevice;
import Ultra.Platform.Renderer.GLRenderDevice;
import Ultra.Platform.Renderer.VKRenderDevice;

namespace Ultra {

Scope<Renderer> Renderer::Create(RenderAPI api) {
    Scope<Renderer> renderer;
    Scope<RenderDevice> device;

    switch (api) {
        case RenderAPI::DirectX: {
            renderer = CreateScope<DXRenderer>();
            device = CreateScope<DXRenderDevice>();
            break;
        }
        case RenderAPI::OpenGL: {
            renderer = CreateScope<GLRenderer>();
            device = CreateScope<GLRenderDevice>();
            break;
        }
        case RenderAPI::Vulkan: {
            renderer = CreateScope<VKRenderer>();
            device = CreateScope<VKRenderDevice>();
            break;
        }
        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer: RenderAPI not supported!");
        #else
            LogFatal("Renderer: RenderAPI not supported!");
        #endif
        }
    }
    renderer->mRenderDevice = std::move(device);
    SetAPI(api);
    renderer->Load();
    return renderer;
}

void Renderer::Load() {
    mRenderDevice->Load();
    mRenderDevice->SetLineThickness(3.0f);
    mCommandBuffer = CommandBuffer::Create();
    mCommandBuffer->SetViewport(0, 0, 800, 600);
    Renderer2D::Load();
}

void Renderer::RenderFrame() {
    mCommandBuffer->Clear(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer2D::ResetStatistics();
}

void Renderer::Dispose() {
    Renderer2D::Dispose();
    mRenderDevice->Dispose();
}

struct CameraData {
    glm::mat4 ViewProjection = {};
    glm::mat4 Projection = {};
    glm::mat4 View = {};
    float NearClip = {};
    float FarClip = {};
};

void Renderer::DrawGrid(const DesignerCamera &camera) {
    static unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    static float vertices[] = {
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
    };
    static CameraData cameraUniformData;

    static Reference<Shader> BasicShader = Shader::Create("Assets/Shaders/Grid.glsl");
    static Reference<PipelineState> BasicPipeline = PipelineState::Create({
        .Layout = {
            { ShaderDataType::Float3, "aPosition" },
        }
    });
    static Reference<Buffer> BasicVertex = Buffer::Create(BufferType::Vertex, vertices, sizeof(vertices));
    static Reference<Buffer> BasicIndex = Buffer::Create(BufferType::Index, indices, sizeof(indices));
    static Reference<Buffer> BasicUniform = Buffer::Create(BufferType::Uniform, nullptr, sizeof(CameraData));
    BasicUniform->Bind(0);

    // Renderer
    BasicShader->Bind();
    //BasicShader->UpdateUniform("uViewProjection", camera.GetViewProjection());
    //BasicShader->UpdateUniform("uProjection", camera.GetProjection());
    //BasicShader->UpdateUniform("uView", camera.GetViewMatrix());
    //BasicShader->UpdateUniform("uNearClip", camera.GetNearPoint());
    //BasicShader->UpdateUniform("uFarClip", camera.GetFarPoint());
    cameraUniformData.Projection = camera.GetProjection();
    cameraUniformData.View = camera.GetViewMatrix();
    cameraUniformData.ViewProjection = camera.GetViewProjection();
    cameraUniformData.NearClip = camera.GetNearPoint();
    cameraUniformData.FarClip = camera.GetFarPoint();
    BasicUniform->UpdateData(&cameraUniformData, sizeof(CameraData));
    BasicVertex->Bind();
    BasicPipeline->Bind();
    BasicIndex->Bind();
    mCommandBuffer->DrawIndexed(6, PrimitiveType::Triangle, true);
}

}
