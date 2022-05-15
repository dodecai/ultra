#pragma once

#include "Omnia/GFX/Context.h"
#include "Omnia/Utility/Property.h"

namespace Omnia {

struct ContextProperties {
	Property<bool> Compatible = false;
	ArithmeticProperty<short> AlphaDepth {0, 8};
	ArithmeticProperty<short> ColorDepth {0, 32};
	ArithmeticProperty<short> DepthBuffer {0, 24};
	ArithmeticProperty<short> StencilBuffer {0, 8};
	ArithmeticProperty<short> MSAABuffer {0, 1};
	ArithmeticProperty<short> MSAASamples {2, 0, 16};  // Depends on hardware so we need to find a way around the crashing.
	ArithmeticProperty<short> VersionMajor {4, 0, 9};
	ArithmeticProperty<short> VersionMinor {6, 0, 9};
};

class GLContext: public Context {
	ContextData *Data;

	bool GetExtensions(int a = 0);
public:
	GLContext(void *window);	// previous CreateContext
	virtual ~GLContext();		// previous DestroyContext

	virtual void Load() override;		// previous LoadGL

	virtual void Attach() override;
	virtual void Detach() override;

	// Accessors
	virtual void *GetNativeContext() override;
	virtual bool const IsCurrentContext() override;

	// Mutators
	virtual void SetViewport(uint32_t width, uint32_t height, int32_t x = 0, int32_t y = 0) override;
	virtual void SwapBuffers() override;

	// Settings
	virtual void SetVSync(bool activate) override;
};

}
