#include <Ultra.h>
#include <EntryPoint.h>

#include "Designer.cpp"

// Application
namespace Ultra {

class Spectra: public Application {
public:
    Spectra(const ApplicationProperties &properties): Application(properties) {
		// Properties
		Renderer::Load();
        //auto &resolution = Application::GetConfig().GetSetting("Designer", "Resolution");
	}

	void Create() override {
		PushLayer(new Designer());
	}

	void Update(Timestamp deltaTime) override {
		// ToDo: Should be executed by default... +Switch to white on light theme Render
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.72f });
		RenderCommand::Clear();
	}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
    return new Ultra::Spectra({ "Spectra", "1280x1024" });
}
