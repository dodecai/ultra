module;

#include <glad/gl.h>

module Ultra.Engine.Renderer;

import Ultra.Engine.RenderCommand;

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

void Renderer::Submit() {
    RenderCommand::DrawIndexed(3, PrimitiveType::Triangle);
}

void Renderer::Unload() {
    Renderer2D::Unload();
}

void Renderer::SetClearColor(float red, float green, float blue, float alpha) {
    RenderCommand::SetClearColor({ red, green, blue, alpha });
}

void Renderer::Resize(const uint32_t width, const uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

}
