#include <Ultra.h>
#include <Ultra/EntryPoint.h>

#include "Layers.cpp"

// Application
namespace Ultra {

class App: public Application {
public:
	App(const ApplicationProperties &properties): Application(properties) {
		Renderer::Load();
	}

	void Create() override {
		PushLayer(new Layer2D());
        //PushLayer(new Layer3D());
	}

	void Destroy() override {}
	void Update(Timestamp deltaTime) override {}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
    return new Ultra::App({ "Ultra", "1280x1024" });
}
