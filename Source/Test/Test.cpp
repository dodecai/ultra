import <Settings.h>;
import <Ultra/EntryPoint.h>;
import Ultra;

import Ultra.Test.Core;
import Ultra.Test.Engine;
import Ultra.Test.Research;

// Switches
//#define CORE_TESTS
//#define ENGINE_TESTS
#define RESEARCH_TESTS

namespace Ultra {

namespace Test {

// Application
class App: public Application {
public:
    // Constructors and Destructor
    App(const ApplicationProperties &properties): Application(properties) {}
    ~App() = default;

    // Methods
    void Create() {
        #ifdef CORE_TESTS
            mCore = CreateReference<Test::Core>();
        #endif
        #ifdef ENGINE_TESTS
            mEngine = CreateReference<Test::Engine>();
        #endif
        #ifdef RESEARCH_TESTS
            mResearch = CreateReference<Test::Research>();
        #endif
    }
    void Destroy() {}
    void Update([[maybe_unused]] Timestamp deltaTime) {
        #ifdef ENGINE_TESTS
            mEngine->Test(deltaTime);
        #endif
    }

private:
    Reference<Test::Core> mCore;
    Reference<Test::Engine> mEngine;
    Reference<Test::Research> mResearch;
};

}

// Application Entry-Point
Application *CreateApplication() {
    return new Test::App({ "Test", "1280x1024" });
}

}
