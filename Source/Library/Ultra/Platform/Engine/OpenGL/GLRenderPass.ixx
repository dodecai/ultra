export module Ultra.Platform.Engine.GLRenderPass;

import Ultra.Platform.GFX.OpenGL;
import Ultra.Engine.RenderPass;

export namespace Ultra {

class GLRenderPass: public RenderPass {
public:
    GLRenderPass() = default;
    GLRenderPass(const RenderPassProperties &properties);
    virtual ~GLRenderPass();

    virtual RenderPassProperties &GetProperties() override { return mProperties; };
    virtual const RenderPassProperties &GetProperties() const override { return mProperties; };

private:
    RendererID mRendererID = 0;
    RenderPassProperties mProperties;
};

}
