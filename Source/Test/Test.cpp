#pragma once

#include <Ultra.h>
#include <Ultra/EntryPoint.h>

// Application
namespace Ultra {

class Test: public Application {
public:
    Test(const ApplicationProperties &properties): Application(properties) {
        Renderer::Load();
    }

    void Create() override {
    }

    void Destroy() override {}
    void Update(Timestamp deltaTime) override {}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
    return new Ultra::Test({ "Ultra-Test", "1280x1024" });
}


#ifdef FUTURE_TEST

static std::future<void> Future;
static void logmillion() {
    Ultra::applog << Ultra::Log::Info << "Started" << "\n";
    for ( size_t i= 0; i <= 1000000; ++i){
        Ultra::applog << "";
    }
    Ultra::applog << Ultra::Log::Info << "Finished " << "\n";
}

void Start() {
    applog << "0: " << std::this_thread::get_id() << std::endl;
    Future = std::async(std::launch::async, logmillion);
    static std::thread MainThread1([&]() { Future = std::async(std::launch::async, logmillion); });
    static std::thread MainThread2([&]() { Future = std::async(std::launch::async, logmillion); });
    static std::thread MainThread3([&]() { Future = std::async(std::launch::async, logmillion); });
    applog << "1: " << MainThread1.get_id() << std::endl;
    applog << "2: " << MainThread2.get_id() << std::endl;
    applog << "3: " << MainThread3.get_id() << std::endl;
}

#endif

#ifdef THIS_IS_A_TEST
#include <iostream>

#define USE_STD_NAMESPACE
#ifdef USE_STD_NAMESPACE
    using namespace std;
#endif

namespace amespace { namespace Level {

namespace Nested::To {
    static inline int Integer = 0;
}

template<typename T>
class Test {
public:
    Test():
        Active { true },
        Counter { 0 },
        ID { "ID" },
        type {} {
    };
    ~Test() = default;

    void DoSomething() { return; }

protected:
    void DoSomethingProtected() { return; }

private:
    void DoSomethingPrivate() { return; }

    bool Active;
    unsigned int Counter;
    string ID;
    T type;
};

void GlobalFunction(int arg = 0) {
    {
        cout << "scope";
    }

    do {
        cout << "do";
    } while (true);

    if (true) {
        cout << true;
    } else {
        cout << false;
    }

    switch (0) {
        case 0: {
            break;
        }

        default: { break; }
    }

    while (true) {
        cout << "while";
    }
}

}}
#endif
