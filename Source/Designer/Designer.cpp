#include <Ultra.h>

// Import Layers
#include "CoreLayer.cpp"

// Application
namespace Ultra {

class Designer: public Application {
public:
	Designer(): Application("Spectra") {
		// Properties
		gladLoadGL();
		Gfx::SetVSync(false);
		Renderer::Load();
	}

	void Create() override {
		PushLayer(new Ultra::CoreLayer());
	}

	void Destroy() override {}

	void Update(Timestamp deltaTime) override {
		// Render
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
	}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
	return new Ultra::Designer();
}
