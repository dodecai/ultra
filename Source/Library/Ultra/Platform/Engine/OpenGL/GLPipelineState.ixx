export module Ultra.Platform.Renderer.GLPipelineState;

import Ultra.Engine.Renderer.PipelineState;

export namespace Ultra {

class GLPipelineState: public PipelineState {
public:
    GLPipelineState(const PipelineProperties &properties);
    virtual ~GLPipelineState();

    virtual void Apply() override;
    virtual void Bind() override;
    virtual void Unbind() override;
};

}
