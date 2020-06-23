#include <Ultra.h>

// Import Layers
#include "Test.cpp"

// Application
namespace Ultra {

class App: public Application {
public:
	App(): Application("Ultra") {
		// Properties
		gladLoadGL();
		Gfx::SetVSync(false);
		Renderer::Load();
	}

	void Create() override {
		//PushLayer(new TestLayer());
		PushLayer(new TestLayer2D());
	}

	void Destroy() override {}

	void Update(Timestamp deltaTime) override {}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
	return new Ultra::App();
}
