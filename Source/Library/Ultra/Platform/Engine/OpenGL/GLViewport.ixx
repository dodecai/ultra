export module Ultra.Platform.Engine.Renderer.GLViewport;

import Ultra.Engine.Renderer.Viewport;

export namespace Ultra {

class GLViewport: public Viewport {
public:
    GLViewport(const ViewportProperties &properties);
    ~GLViewport() = default;
};

}
