export module Ultra.Platform.GFX.GLContext;

import Ultra.Core;
import Ultra.GFX.Context;
import Ultra.Utility.Property;

import Ultra.Platform.GFX.OpenGL;

export namespace Ultra {

struct ContextProperties {
    Property<bool> Compatible = false;
    ArithmeticProperty<short> AlphaDepth { 0, 8 };
    ArithmeticProperty<short> ColorDepth { 0, 32 };
    ArithmeticProperty<short> DepthBuffer { 0, 24 };
    ArithmeticProperty<short> StencilBuffer { 0, 8 };
    ArithmeticProperty<short> MSAABuffer { 0, 1 };
    ArithmeticProperty<short> MSAASamples { 2, 0, 16 };  // ToDo: Depends on hardware so we need to find a way around the crashing.
    ArithmeticProperty<short> VersionMajor { 4, 0, 9 };
    ArithmeticProperty<short> VersionMinor { 6, 0, 9 };
};

class GLContext: public Context {
public:
    // Default
    GLContext(void *window);
    virtual ~GLContext();
    virtual void Load() override;

    // Controls
    virtual void Attach() override;
    virtual void Detach() override;
    virtual void Clear() override;
    virtual void SwapBuffers() override;

    // Accessors
    virtual void *GetNativeContext() override;

    // Mutators
    virtual void SetViewport(uint32_t width, uint32_t height, int32_t x = 0, int32_t y = 0) override;
    virtual void SetVSync(bool activate) override;

private:
    // Methods
    virtual bool const IsCurrentContext() override;

    // Properties
    ContextData *Data;
    bool GetExtensions(int a = 0);
};

}
