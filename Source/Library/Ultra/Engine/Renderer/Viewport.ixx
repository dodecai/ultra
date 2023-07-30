export module Ultra.Engine.Renderer.Viewport;

import Ultra.Core;
import Ultra.Logger;

export namespace Ultra {

struct ViewportProperties {
    float X;
    float Y;
    float Width;
    float Height;

    bool Window;
};

class Viewport {
public:
    Viewport(const ViewportProperties &properties):
        mProperties(properties),
        mAspectRatio(properties.Width / properties.Height) {
    }
    virtual ~Viewport() = default;
    static Scope<Viewport> Create(const ViewportProperties &properties);

    float GetAspectRatio() const {
        return mAspectRatio;
    }
    const ViewportProperties &GetProperties() const {
        return mProperties;
    }

private:
    ViewportProperties mProperties {};
    float mAspectRatio {};
};

}
