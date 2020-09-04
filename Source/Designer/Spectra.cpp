#include <Ultra.h>
#include <Ultra/EntryPoint.h>

#include "Spectra/Designer.cpp"

// Application
namespace Ultra {

class Spectra: public Application {
public:
    Spectra(const ApplicationProperties &properties): Application(properties) {
        auto &resolution = Application::GetConfig().GetSetting("Designer", "Resolution");
		Renderer::Load();
	}

	void Create() override {
		PushLayer(new Designer());
	}

	void Update(Timestamp deltaTime) override {
		// ToDo: Should be executed by default... +Switch to white on light theme Render
		RenderCommand::Clear();
	}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
    return new Ultra::Spectra({ "Spectra", "1280x1024" });
}
