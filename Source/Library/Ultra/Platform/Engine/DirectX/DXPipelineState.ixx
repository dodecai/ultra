export module Ultra.Platform.Renderer.DXPipelineState;

import Ultra.Renderer.PipelineState;

export namespace Ultra {

class DXPipelineState: public PipelineState {
public:
    DXPipelineState(CullMode cullMode, BlendMode blendMode);
    virtual ~DXPipelineState();

    virtual void Apply() override;
};

}
