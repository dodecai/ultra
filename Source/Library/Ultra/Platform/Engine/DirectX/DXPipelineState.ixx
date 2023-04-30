export module Ultra.Platform.Renderer.DXPipelineState;

import Ultra.Engine.Renderer.PipelineState;

export namespace Ultra {

class DXPipelineState: public PipelineState {
public:
    DXPipelineState(const PipelineProperties &properties);
    virtual ~DXPipelineState();

    virtual void Apply() override;
    virtual void Bind() override;
    virtual void Unbind() override;
};

}
