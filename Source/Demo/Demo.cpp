#include <Ultra.h>

// Application
namespace Ultra {

class Demo: public Application {
public:
	Demo(): Application("Ultra-Demo") {}
	void Create() override {}
	void Destroy() override {}
	void Update(Timestamp deltaTime) override {}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
	return new Ultra::Demo();
}
