export module Ultra.Platform.Renderer.GLPipelineState;

import Ultra.Engine.Renderer.PipelineState;

export namespace Ultra {

class GLPipelineState: public PipelineState {
public:
    GLPipelineState(const PipelineProperties &properties);
    virtual ~GLPipelineState();

    void Apply() override;
    void Bind() override;
    void Unbind() override;

private:
    void ResetProperties();
    void UpdateProperties();
};

}
