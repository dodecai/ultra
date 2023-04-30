export module Ultra.Platform.Renderer.SWPipelineState;

import Ultra.Renderer.PipelineState;

export namespace Ultra {

class SWPipelineState: public PipelineState {
public:
    SWPipelineState(const PipelineProperties &properties);
    virtual ~SWPipelineState();

    virtual void Apply() override;
    virtual void Bind() override;
    virtual void Unbind() override;
};

}
