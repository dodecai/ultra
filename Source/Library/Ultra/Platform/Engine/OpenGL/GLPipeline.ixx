export module Ultra.Platform.Engine.GLPipeline;

import Ultra.Platform.GFX.OpenGL;
import Ultra.Engine.Pipeline;

export namespace Ultra {

class GLPipeline: public Pipeline {
public:
    GLPipeline() = default;
    GLPipeline(const PipelineProperties &properties);
    virtual ~GLPipeline();

    virtual void Bind() const override;
    virtual void Invalidate() override;

    virtual PipelineProperties &GetProperties() override { return mProperties; };
    virtual const PipelineProperties &GetProperties() const override { return mProperties; };

private:
    PipelineProperties mProperties;
    RendererID mRendererID = 0;
};

}
