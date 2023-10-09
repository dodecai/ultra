export module Ultra.Renderer.CommandBuffer;

import Ultra.Core;
import Ultra.Logger;

import Ultra.Renderer.Buffer;
import Ultra.Renderer.Framebuffer;
import Ultra.Renderer.PipelineState;
import Ultra.Renderer.RenderDevice;
import Ultra.Renderer.Shader;
import Ultra.Renderer.Texture;

export namespace Ultra {

/// 
/// @brief Agnostic CommandBuffer
/// 
/// @example: How-To
/// auto commands = CommandBuffer::Create();
/// 
/// commands->Begin();
/// ... Record Commands (e.g. SetViewPort, bind Shaders, Buffers, PipelineStates, etc)
/// commands->End();
/// commands->Execute();
///
class CommandBuffer {
public:
    CommandBuffer() = default;
    virtual ~CommandBuffer() = default;

    static Scope<CommandBuffer> Create();

    virtual void Begin() = 0;
    virtual void End() = 0;

    virtual void Clear(float r, float g, float b, float a) = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    //virtual void BindRenderState(const Scope<RenderState> &renderState) = 0;
    //virtual void BindShader(const Scope<Shader> &shader) = 0;
    //virtual void BindVertexBuffer(const Scope<Buffer> &vertexBuffer) = 0;
    //virtual void BindIndexBuffer(const Scope<Buffer> &indexBuffer) = 0;
    //virtual void BindFramebuffer(const Scope<Framebuffer> &framebuffer) = 0;

    virtual void Draw(uint32_t vertexCount, uint32_t instanceCount = 0, uint32_t firstVertex = 0, uint32_t firstInstance = 0) = 0;
    virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 0, uint32_t firstIndex = 0, int32_t vertexOffset = 0, uint32_t firstInstance = 0) = 0;
    virtual void DrawIndexed(size_t count, PrimitiveType type, bool depthTest = true) = 0;

    virtual void Execute() = 0;
};

}
