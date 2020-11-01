#include <Ultra.h>
#include <Ultra/EntryPoint.h>

#include "Spectra/Designer.h"

// Application
namespace Ultra {

class Spectra: public Application {
public:
    Spectra(const ApplicationProperties &properties): Application(properties) {
        auto resolution = Application::GetConfig().GetSetting("Designer", "Resolution");
        Renderer::Load();
	}

	void Create() override {
        PushLayer(new Designer());
	}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
    return new Ultra::Spectra({ "Spectra", "1280x1024" });
}
