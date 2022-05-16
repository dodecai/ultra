#include <Settings.h>
#include <Ultra/EntryPoint.h>

import Ultra;

namespace Ultra {

// Application
class App: public Application {
public:
    // Constructors and Destructor
    App() = default;
    ~App() = default;

    // Methods
    void Create() {
        Test();
    }

    void Destroy() {
    }

    void Update() {
        Exit();
    }

    void Test() {
        logger.Location() << "Test" << std::endl;
    }
};

// Application Entry-Point
Application *CreateApplication() {
    return new App();
}

}
