#include <Ultra.h>
#include <Ultra/EntryPoint.h>

#include "Layers.cpp"

// Application
namespace Ultra {

class Demo: public Application {
public:
	Demo(const ApplicationProperties &properties): Application(properties) {
		Renderer::Load();
	}

	void Create() override {
		PushLayer(new Game2D());
        //PushLayer(new Game3D());
	}

	void Destroy() override {}
	void Update(Timestamp deltaTime) override {}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
    return new Ultra::Demo({ "Ultra-Demo", "1280x1024" });
}
