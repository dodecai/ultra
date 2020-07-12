#include <Ultra.h>
#include <EntryPoint.h>

#include "Game2D.cpp"

// Application
namespace Ultra {

class Demo: public Application {
public:
	Demo(): Application("Ultra-Demo") {
		// Properties
		//gladLoadGL();
		//Gfx::SetVSync(false);
		Renderer::Load();
	}
	void Create() override {
		PushLayer(new Game2D());
	}

	void Destroy() override {}
	void Update(Timestamp deltaTime) override {}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
	return new Ultra::Demo();
}
