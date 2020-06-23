#include <Ultra.h>

namespace Ultra {

class CoreLayer: public Layer {
public:
	CoreLayer():
		Layer("Core") {
	}

	void Create() override {}

	void GuiUpdate() override {
		ImGui::Begin("Settings");
		ImGui::Text("Spectra");
		ImGui::End();
	}

	void Update(Timestamp deltaTime) override {}
};

}
