module Ultra.Engine.Renderer;

import Ultra.Engine.RenderCommand;
import Ultra.Engine.Renderer2D;

namespace Ultra {

Renderer::SceneData *Renderer::mSceneData = new Renderer::SceneData;

struct RendererData {
    //Reference<RenderPass> ActiveRenderPass;
};

void Renderer::Load() {
    RenderCommand::Load();
    Renderer2D::Load();
}

void Renderer::BeginScene() {
    RenderCommand::Clear();
    //mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {
}

void Renderer::Resize(const uint32_t width, const uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

}
