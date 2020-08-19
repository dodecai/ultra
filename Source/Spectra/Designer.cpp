#include <Ultra.h>
#include <EntryPoint.h>

#include "MainLayer.cpp"

// Application
namespace Ultra {

class Designer: public Application {
public:
	Designer(): Application("Spectra") {
		// Properties
		Renderer::Load();
	}

	void Create() override {
		PushLayer(new Ultra::MainLayer());
	}

	void Destroy() override {}
	void Update(Timestamp deltaTime) override {
		// Render
		// ToDo: Switch to white on light theme
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.72f });
		RenderCommand::Clear();
	}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
	return new Ultra::Designer();
}
