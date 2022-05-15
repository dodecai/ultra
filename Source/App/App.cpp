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
        logger << "Create\n";
    }

    void Destroy() {
        logger << "Destroy\n";
    }

    void Update() {
        logger << "Update\n";
        Exit();
    }
};

// Application Entry-Point
Application *CreateApplication() {
    return new App();
}

}
