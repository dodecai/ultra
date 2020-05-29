#include <Ultra.h>
#include <Omnia/EntryPoint.h>

class Sandbox: public Omnia::Application {
public:
	Sandbox() {
	}

	~Sandbox() {
	}
};


Omnia::Application *Omnia::CreateApplication() {
	return new Sandbox();
}
