#include <Ultra.h>
#include <Ultra/EntryPoint.h>

//#define RAW_RENDERER_TEST
#define VULKAN_TESTS1

#include "Spectra/Designer.cpp"

// Application
namespace Ultra {

class Spectra: public Application {
public:
    Spectra(const ApplicationProperties &properties): Application(properties) {
        auto &resolution = Application::GetConfig().GetSetting("Designer", "Resolution");
        #ifndef VULKAN_TESTS
        Renderer::Load();
        #endif
	}

	void Create() override {
        PushLayer(new Designer());
	}

	void Update(Timestamp deltaTime) override {
		// ToDo: Should be executed by default... +Switch to white on light theme Render
        #ifndef VULKAN_TESTS
		RenderCommand::Clear();
        #endif
	}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
    return new Ultra::Spectra({ "Spectra", "1280x1024" });
}
