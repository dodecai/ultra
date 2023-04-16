export module Ultra.Platform.Renderer.GLPipelineState;

import Ultra.Renderer.PipelineState;

export namespace Ultra {

class GLPipelineState: public PipelineState {
public:
    GLPipelineState(CullMode cullMode, BlendMode blendMode);
    virtual ~GLPipelineState();

    virtual void Apply() override;
};

}
