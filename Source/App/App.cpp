#include <Ultra.h>

// Applicaton
namespace Ultra {

class MainLayer: public Layer {
public:
	MainLayer():
		Layer{ "Main" } {
	}

	void Event(void *event) {}
	void Update() override {}
};

class App: public Application {
public:
	void Create() {
		// Preparation
		applog <<  "Welcome to " << AppCaption << "!" << std::endl;
		PushLayer(new MainLayer());
		PushOverlay(new GuiLayer());
	}

	void Destroy() {}
	void Update() {}

	//void KeyboardEvent(KeyboardEventData data) {}
	//void MouseEvent(MouseEventData data) {}
	//void WindowEvent(WindowEventData data) {}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
	return new Ultra::App();
}
