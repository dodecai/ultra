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
    void Create() { Test(); }
    void Destroy() {}
    void Update(Timestamp deltatime) {
        Exit();
    }

    void Test() {
        Cli::Test();
        Logger::Test();

        logger << apptime.GetDate() << "\n";
        logger << apptime.GetTime() << "\n";
        logger << apptime.GetTimeStamp() << "\n";

        auto now = apptime.Now();
        auto ns = apptime.Now().AsNanoseconds();
        auto µs = apptime.Now().AsMicroseconds();
        auto ms = apptime.Now().AsMilliseconds();
        auto s = apptime.Now().AsSeconds();

        ns = apptime.GetNanoseconds(ns.count());
    }
};

// Application Entry-Point
Application *CreateApplication() {
    return new App();
}

}
