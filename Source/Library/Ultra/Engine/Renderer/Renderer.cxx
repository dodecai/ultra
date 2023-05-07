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


void Renderer::Test() {
    static float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    static uint32_t indices[] = { 0, 1, 2 };

    static Reference<Shader> BasicShader = Shader::Create("Assets/Shaders/Sample.glsl");
    static Reference<PipelineState> BasicPipeline = PipelineState::Create({
        VertexBufferLayout {
            { ShaderDataType::Float3, "position" }
        }
    });
    static Reference<Buffer> BasicVertex;
    static Reference<Buffer> BasicIndex;

    BasicVertex = Buffer::Create(BufferType::Vertex, vertices, sizeof(vertices));
    BasicIndex = Buffer::Create(BufferType::Index, indices, sizeof(indices));

    // Renderer
    BasicShader->Bind();
    BasicVertex->Bind();
    BasicPipeline->Bind();
    BasicIndex->Bind();
    mCommandBuffer->DrawIndexed(3);
}

}
