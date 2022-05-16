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
        Ultra::Cli::Test();
        logger << LogLevel::Info << "Create\n";
    }

    void Destroy() {
        logger << LogLevel::Warn << "Destroy\n";
        logger << logger.GetCounter() << std::endl;
    }

    void Update() {
        logger << LogLevel::Error << "Update\n";
        Exit();
    }
};

// Application Entry-Point
Application *CreateApplication() {
    return new App();
}

}
