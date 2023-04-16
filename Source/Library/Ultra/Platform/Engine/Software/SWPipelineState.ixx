export module Ultra.Platform.Renderer.SWPipelineState;

import Ultra.Renderer.PipelineState;

export namespace Ultra {

class SWPipelineState: public PipelineState {
public:
    SWPipelineState(CullMode cullMode, BlendMode blendMode);
    virtual ~SWPipelineState();

    virtual void Apply() override;
};

}
