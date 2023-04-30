export module Ultra.Platform.Renderer.VKPipelineState;

import Ultra.Engine.Renderer.PipelineState;

export namespace Ultra {

class VKPipelineState: public PipelineState {
public:
    VKPipelineState(const PipelineProperties &properties);
    virtual ~VKPipelineState();

    virtual void Apply() override;
    virtual void Bind() override;
    virtual void Unbind() override;
};

}
