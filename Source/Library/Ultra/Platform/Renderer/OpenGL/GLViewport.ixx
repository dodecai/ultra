export module Ultra.Platform.Renderer.GLViewport;

import Ultra.Renderer.Viewport;

export namespace Ultra {

class GLViewport: public Viewport {
public:
    GLViewport(const ViewportProperties &properties);
    ~GLViewport() = default;
};

}
