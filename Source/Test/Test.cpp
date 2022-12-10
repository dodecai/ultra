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
    void Create() {}
    void Destroy() {}
    void Update(Timestamp deltatime) {
        Test();
        //Exit();
    }

    void Test() {
        //Cli::Test();
        //Logger::Test();

        //logger << apptime.GetDate() << "\n";
        //logger << apptime.GetTime() << "\n";
        //logger << apptime.GetTimeStamp() << "\n";
    }
};

// Application Entry-Point
Application *CreateApplication() {
    return new App();
}

}
