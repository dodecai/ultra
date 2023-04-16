export module Ultra.Platform.Renderer.VKPipelineState;

import Ultra.Renderer.PipelineState;

export namespace Ultra {

class VKPipelineState: public PipelineState {
public:
    VKPipelineState(CullMode cullMode, BlendMode blendMode);
    virtual ~VKPipelineState();

    virtual void Apply() override;
};

}
