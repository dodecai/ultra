#include <Settings.h>
#include <Ultra/EntryPoint.h>

import Ultra;

namespace Ultra {

struct KeyboardEvent {
    int KeyCode = 0;
};

struct MouseEvent {
    int X = 0;
    int Y = 0;
};

struct EventListenerTest {
    void OnKeyboardEvent(const KeyboardEvent &data) {
        logger << __FUNCTION__ << ":" << data.KeyCode << "\n";
    }
    void OnMouseEvent(const MouseEvent &data) {
        logger << __FUNCTION__ << ":" << data.X << "\n";
    }
};

struct Tester {
    int Value = 5;

    //void BasicEvent(string test) {
    //    logger << __FUNCTION__ << ":" << test << "\n";
    //}

    //void BasicEventInt(int test) {
    //    logger << __FUNCTION__ << ":" << test << "\n";
    //}

    bool Event(string test) {
        logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    const bool ConstEvent(string test) {
        logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    static bool StaticEvent(string test) {
        logger << __FUNCTION__ << ":" << test << "\n";
        return true;
    }

    bool NoExceptEvent(string test) noexcept {
        logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    const bool NoExceptConstEvent(string test) noexcept {
        logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    static bool NoExceptStaticEvent(string test) noexcept {
        logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    volatile bool VolatileEvent(string test) {
        logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    const volatile bool VolatileConstEvent(string test) {
        logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    static volatile bool VolatileStaticEvent(string test) {
        logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    static const volatile bool VolatileStaticConstEvent(string test) {
        logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }
};

// Application
class App: public Application {
    struct EventEmitter: Emitter<EventEmitter> {};

public:
    // Constructors and Destructor
    App() = default;
    ~App() = default;

    // Methods
    void Create() {
        Test();
    }
    void Destroy() {}
    void Update(Timestamp deltaTime) {}

    void Test() {
  
        // DateTime
        logger << "DateTime" << "\n";
        logger << "CurrentDate: " << apptime.GetDate() << "\n";
        logger << "CurrentTime: " << apptime.GetTime() << "\n";
        logger << "CurrentTimestamp: " << apptime.GetTimeStamp() << "\n";
        logger << "CurrentRuntime: " << apptime.GetRuntime() << "\n";
        //for (size_t i = 0; i < 10'000'000; i++) {
        //    auto test = new string("");
        //    delete test;
        //}
        logger << "CurrentRuntime: " << apptime.GetRuntime() << "\n";

        // Timer
        //logger << "Timer" << "\n";
        //Timer timer {};
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Duration: " << timer.GetDeltaTime() << "\n";
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Duration(ns): " << timer.GetDeltaTimeAs(TimerUnit::Nanoseconds) << "\n";;
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Duration(µs): " << timer.GetDeltaTimeAs(TimerUnit::Microseconds) << "\n";;
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Duration(ms): " << timer.GetDeltaTimeAs(TimerUnit::Milliseconds) << "\n";;
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Now: " << timer.Now() << "\n";
        //logger << "Duration(s): " << timer.GetDeltaTimeAs(TimerUnit::Seconds) << "\n";;

        // UUID
        //logger << "UUID" << "\n";
        //logger << UUID<uint16_t>() << "\n";
        //logger << UUID<uint32_t>() << "\n";
        //logger << UUID<uint64_t>() << "\n";
        //logger << UUID() << "\n";

        return;


        //auto a = "Test";
        ////Cli::Test();
        ////Logger::Test();

        //// Events
        ////Dispatcher<bool(string)> TestA;
        //auto timer = Timer();
        //{
        //    Tester *tester = new Tester();
        //    
        //    // Delegates
        //    Delegate<bool(string)> delegateA {};
        //    Delegate<bool(string)> delegateB {};

        //    delegateB.connect<&Tester::StaticEvent>();
        //    delegateA.connect<&Tester::Event>(tester);

        //    auto resultB = delegateB("DelegateA");
        //    auto resultA = delegateA("DelegateB");

        //    // Signals
        //    Signal<bool(string)> signalA;
        //    Sink sinkA { signalA };

        //    sinkA.connect<&Tester::VolatileStaticEvent>();
        //    sinkA.connect<&Tester::VolatileStaticConstEvent>();

        //    signalA.publish("Signal");

        //    // Dispatcher
        //    Dispatcher dispatcherA {};

        //    EventListenerTest listener;
        //    dispatcherA.sink<KeyboardEvent>().connect<&EventListenerTest::OnKeyboardEvent>(listener);
        //    dispatcherA.sink<MouseEvent>().connect<&EventListenerTest::OnMouseEvent>(listener);

        //    dispatcherA.trigger(KeyboardEvent { 60 });
        //    dispatcherA.enqueue<KeyboardEvent>(65);
        //    dispatcherA.enqueue<KeyboardEvent>(67);
        //    dispatcherA.update<KeyboardEvent>();
        //    dispatcherA.trigger(MouseEvent { -1 });

        //    // Emitter
        //    EventEmitter emitterA {};
        //    emitterA.on<KeyboardEvent>([](const KeyboardEvent &data, EventEmitter &emitter) {
        //        logger << __FUNCTION__ << ": Emitter " << data.KeyCode << "\n";
        //    });
        //    emitterA.publish(KeyboardEvent { 42 });
        //    emitterA.publish(KeyboardEvent { 43 });

        //    //auto test = DelegateMember(*tester, &Tester::Event);
        //    //test("1");


        //    //auto test2 = DelegateMember(*tester, &Tester::ConstEvent);
        //    //test2("2");

        //    //auto test3 = Delegate(&Tester::StaticEvent);
        //    //test3("3");

        //    //auto same = test == test2;
        //    delete tester;

        //    // Lambda
        //    //TestA += [&](string test) -> bool {
        //    //    logger << __FUNCTION__ << ":" << test << "\n";
        //    //    return false;
        //    //};
        //    //TestA += [&](string test) -> bool {
        //    //    logger << __FUNCTION__ << ":" << test << "\n";
        //    //    return true;
        //    //};

        //    // Members
        //    //TestA += DelegateMember(*tester, &Tester::Event);
        //    //TestA += DelegateMember(*tester, &Tester::Event);
        //    //TestA += DelegateMember(*tester, &Tester::ConstEvent);
        //    //TestA += DelegateMember(*tester, &Tester::ConstEvent);
        //    //TestA += DelegateMember(*tester, &Tester::NoExceptEvent);
        //    //TestA += DelegateMember(*tester, &Tester::NoExceptEvent);
        //    //TestA += DelegateMember(*tester, &Tester::NoExceptConstEvent);
        //    //TestA += DelegateMember(*tester, &Tester::NoExceptConstEvent);
        //    //TestA += DelegateMember(*tester, &Tester::VolatileEvent);
        //    //TestA += DelegateMember(*tester, &Tester::VolatileEvent);
        //    //TestA += DelegateMember(*tester, &Tester::VolatileConstEvent);
        //    //TestA += DelegateMember(*tester, &Tester::VolatileConstEvent);

        //    // Functions
        //    //TestA += &Tester::StaticEvent;
        //    //TestA += &Tester::StaticEvent;
        //    //TestA += &Tester::NoExceptStaticEvent;
        //    //TestA += &Tester::NoExceptStaticEvent;
        //    //TestA += &Tester::VolatileStaticEvent;
        //    //TestA += &Tester::VolatileStaticEvent;
        //    //TestA += &Tester::VolatileStaticConstEvent;
        //    //TestA += &Tester::VolatileStaticConstEvent;
        //    //VerifyMemoryUsage();
        //    logger << "Duration: " << timer.GetDeltaTime() << "\n";
        //}
        ////TestA("Test A");
        //logger << "Duration: " << timer.GetDeltaTime() << "\n";
    }
};

// Application Entry-Point
Application *CreateApplication() {
    return new App();
}

}
