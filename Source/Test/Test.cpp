import <Settings.h>;
import <Ultra/EntryPoint.h>;
import Ultra;
import Ultra.Core.ThreadPool;
import Ultra.Asset.Manager;
import Ultra.Renderer.DesignerCamera;

// Switches
#define ENGINE_TESTS
//#define LIBRARY_TESTS
//#define MISCELLANEOUS_TESTS

namespace Ultra {

#pragma region Events

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
        //logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    const bool ConstEvent(string test) {
        //logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    static bool StaticEvent(string test) {
        //logger << __FUNCTION__ << ":" << test << "\n";
        return true;
    }

    bool NoExceptEvent(string test) noexcept {
        //logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    const bool NoExceptConstEvent(string test) noexcept {
        //logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    static bool NoExceptStaticEvent(string test) noexcept {
        //logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    volatile bool VolatileEvent(string test) {
        //logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    const volatile bool VolatileConstEvent(string test) {
        //logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    static volatile bool VolatileStaticEvent(string test) {
        //logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }

    static const volatile bool VolatileStaticConstEvent(string test) {
        //logger << __FUNCTION__ << ":" << test << "\n";
        return false;
    }
};

#pragma endregion

#ifdef FUTURE_TEST

static std::future<void> Future;
static void logmillion() {
    Ultra::applog << Ultra::Log::Info << "Started" << "\n";
    for (size_t i = 0; i <= 1000000; ++i) {
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

// Application
class App: public Application {
    struct EventEmitter: Emitter<EventEmitter> {};

public:
    // Constructors and Destructor
    App(const ApplicationProperties &properties): Application(properties) {
        AssetManager::Instance().Load();
    }
    ~App() = default;

    // Methods
    void Create() {
        #ifdef ENGINE_TESTS
            mRenderer = Renderer::Create();
            auto swapchain = Swapchain::Create(nullptr, 1280, 1024);

            auto aspectRatio = 1280.0f / 1024.0f;
            mDesignerCamera = DesignerCamera(45.0f, aspectRatio, 0.1f, 1000.0f);

            auto commandBuffer = CommandBuffer::Create();
            mCheckerBoard = Texture::Create(TextureProperties(), "./Assets/Textures/CheckerBoard.png");
        #endif
        #ifdef LIBRARY_TESTS
            LibraryTest();
        #endif
        #ifdef MISCELLANEOUS_TESTS
            Test();
        #endif
    }
    void Destroy() {}
    void Update([[maybe_unused]] Timestamp deltaTime) {
        #ifdef ENGINE_TESTS
            EngineTest(deltaTime);
        #endif
    }

    ///
    /// @brief Engine Tests
    ///
    void EngineTest(Timestamp deltaTime) {
        mRenderer->RenderFrame();
        mDesignerCamera.Update(deltaTime);
        Renderer2D::StartScene(mDesignerCamera);

        // 3D Renderer: Primitives
        mRenderer->Test(mDesignerCamera);
        //mRenderer->DrawGrid(mDesignerCamera);

        // 2D Renderer: Primitives
        //Renderer2D::DrawLine({ -0.9f, -0.9f }, {  0.9f,  -0.9f }, { 0.9f, 0.9f, 0.9f, 1.0f });
        //Renderer2D::DrawLine({ -0.9f, -0.9f }, { -0.9f,   0.9f }, { 1.0f, 0.0f, 1.0f, 1.0f });
        //Renderer2D::DrawLine({  0.2f,  0.2f }, {  0.7f,   0.7f }, { 1.0f, 1.0f, 1.0f, 1.0f });
        //Renderer2D::DrawQuad({ -0.6f, -0.6f }, {  0.5f,   0.5f }, mCheckerBoard, 1.0f, { 1.0f, 0.0f, 0.0f, 1.0f });
        //Renderer2D::DrawQuad({  0.2f,  0.2f }, {  0.7f,   0.7f }, mCheckerBoard, 1.0f, { 0.0f, 0.0f, 1.0f, 1.0f });
        //Renderer2D::DrawRect({ -0.9f,  0.9f }, {  0.5f,   0.5f }, { 0.2f, 0.2f, 0.2f, 1.0f });
        //Renderer2D::DrawCircle({ 1.0f, 1.0f }, {  0.5f,   0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, 0.1f, 0.1f);

        //static float rotation = 0.0f;
        //const float speed = 180.0f;
        //rotation += speed * deltaTime;
        //if (rotation >= 360.0f) rotation = 0.0f;
        //Renderer2D::DrawRotatedQuad({  0.7f,   0.7f }, { 0.2f,  0.2f }, rotation * (3.14f / 180.0f), mCheckerBoard, 1.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
        //Renderer2D::DrawRotatedQuad({  0.7f,  -0.6f }, { 0.4f,  0.4f }, rotation * (3.14f / 180.0f) * -1.0f, mCheckerBoard, 1.0f, { 0.7f, 0.7f, 0.7f, 1.0f });

        //// Finish
        //Renderer2D::FinishScene();
    }

    ///
    /// @brief Library Tests
    ///
    void LibraryTest() {
        return;
        auto shaders = Directory::GetFiles("Assets/Shaders", ".glsl");
        auto bin = File::LoadAsBinary("Assets/Shaders/Test.glsl");
        auto txt = File::LoadAsText("Assets/Shaders/Test.glsl");
        return; // Heap Corruption


        ///
        /// Core
        ///
        LogCaption("Core");
        // Property
        Log("Property");
        LogDelimiter("");
        LogTrace("PropertySize before: {}", static_cast<size_t>(PropertySize));
        PropertySize = 1001;
        LogTrace("PropertySize after setting: {}", static_cast<size_t>(PropertySize));
        LogDelimiter("");
        // UUID
        Log("UUID");
        LogDelimiter("");
        LogInfo("UUID [uint16]: {}", static_cast<uint16_t>(UUID<uint16_t>()));
        LogInfo("UUID [uint32]: {}", static_cast<uint32_t>(UUID<uint32_t>()));
        LogInfo("UUID [uint64]: {}", static_cast<uint64_t>(UUID<uint64_t>()));
        LogInfo("UUID [string]: {}", static_cast<string>(UUID()));
        LogDelimiter("");
        // Logger
        Log("Logger");
        LogDelimiter("");
        logger << LogLevel::Trace << "Hello World! 🦄" << "\n";
        LogTrace("{}: {} {:.2}", "Hello", "World! 🦄", 1.234567f);
        Logger::Test();
        LogDelimiter("");

        ///
        /// Math
        ///
        LogCaption("Math");
        // Matrix
        // Quaternion
        // Vector
        Log("Vector");
        LogDelimiter("");
        Test::VectorTests();
        LogDelimiter("");

        ///
        /// System
        ///
        LogCaption("System");
        // CLI
        Log("CLI");
        LogDelimiter("");
        Cli::Test();
        LogDelimiter("");
        
        ///
        /// Utilities
        ///
        LogCaption("Utilities");
        // DateTime
        Log("DateTime");
        LogDelimiter("");
        logger(" - CurrentDate:      {}\n", apptime.GetDate());
        logger(" - CurrentTime:      {}\n", apptime.GetTime());
        logger(" - CurrentTimestamp: {}\n", apptime.GetTimeStamp());
        logger(" - CurrentRuntime:   {}\n", apptime.GetRuntime());
        LogDelimiter("");

        // Delegate, Dispatcher, Emitter and Signal
        Log("Delegate, Dispatcher, Emitter and Signal");
        LogDelimiter("");
        //Dispatcher<bool(string)> TestA;
        auto timerE = Timer();
        {
            Tester *tester = new Tester();

            // Delegates
            Delegate<bool(string)> delegateA {};
            Delegate<bool(string)> delegateB {};

            delegateB.connect<&Tester::StaticEvent>();
            delegateA.connect<&Tester::Event>(tester);

            auto resultB = delegateB("DelegateA");
            auto resultA = delegateA("DelegateB");

            // Signals
            //Signal<bool(string)> signalA;
            //Sink sinkA { signalA };

            //sinkA.connect<&Tester::VolatileStaticEvent>();
            //sinkA.connect<&Tester::VolatileStaticConstEvent>();

            //signalA.publish("Signal");

            // Dispatcher
            Dispatcher dispatcherA {};

            EventListenerTest listener;
            dispatcherA.sink<KeyboardEvent>().connect<&EventListenerTest::OnKeyboardEvent>(listener);
            dispatcherA.sink<MouseEvent>().connect<&EventListenerTest::OnMouseEvent>(listener);

            dispatcherA.trigger(KeyboardEvent { 60 });
            dispatcherA.enqueue<KeyboardEvent>(65);
            dispatcherA.enqueue<KeyboardEvent>(67);
            dispatcherA.update<KeyboardEvent>();
            dispatcherA.trigger(MouseEvent { -1 });

            // Emitter
            EventEmitter emitterA {};
            emitterA.on<KeyboardEvent>([](const KeyboardEvent &data, EventEmitter &emitter) {
                logger << __FUNCTION__ << ": Emitter " << data.KeyCode << "\n";
            });
            emitterA.publish(KeyboardEvent { 42 });
            emitterA.publish(KeyboardEvent { 43 });

            //auto test = DelegateMember(*tester, &Tester::Event);
            //test("1");


            //auto test2 = DelegateMember(*tester, &Tester::ConstEvent);
            //test2("2");

            //auto test3 = Delegate(&Tester::StaticEvent);
            //test3("3");

            //auto same = test == test2;
            delete tester;

            // Lambda
            //TestA += [&](string test) -> bool {
            //    logger << __FUNCTION__ << ":" << test << "\n";
            //    return false;
            //};
            //TestA += [&](string test) -> bool {
            //    logger << __FUNCTION__ << ":" << test << "\n";
            //    return true;
            //};

            // Members
            //TestA += DelegateMember(*tester, &Tester::Event);
            //TestA += DelegateMember(*tester, &Tester::Event);
            //TestA += DelegateMember(*tester, &Tester::ConstEvent);
            //TestA += DelegateMember(*tester, &Tester::ConstEvent);
            //TestA += DelegateMember(*tester, &Tester::NoExceptEvent);
            //TestA += DelegateMember(*tester, &Tester::NoExceptEvent);
            //TestA += DelegateMember(*tester, &Tester::NoExceptConstEvent);
            //TestA += DelegateMember(*tester, &Tester::NoExceptConstEvent);
            //TestA += DelegateMember(*tester, &Tester::VolatileEvent);
            //TestA += DelegateMember(*tester, &Tester::VolatileEvent);
            //TestA += DelegateMember(*tester, &Tester::VolatileConstEvent);
            //TestA += DelegateMember(*tester, &Tester::VolatileConstEvent);

            // Functions
            //TestA += &Tester::StaticEvent;
            //TestA += &Tester::StaticEvent;
            //TestA += &Tester::NoExceptStaticEvent;
            //TestA += &Tester::NoExceptStaticEvent;
            //TestA += &Tester::VolatileStaticEvent;
            //TestA += &Tester::VolatileStaticEvent;
            //TestA += &Tester::VolatileStaticConstEvent;
            //TestA += &Tester::VolatileStaticConstEvent;
            //VerifyMemoryUsage();
            Log("Duration: {}", timerE.GetDeltaTime());
        }
        //TestA("Test A");
        Log("Duration: {}", timerE.GetDeltaTime());
        // String
        Log("String");
        LogDelimiter("");
        //String::Test();
        LogDelimiter("");
        // ThreadPool
        Log("ThreadPool");
        LogDelimiter("");
        ThreadPool pool;
        for (int i = 0; i < 8; ++i) {
            // Create eight tasks in the queue
            pool.Enqueue([i] {
                auto timer = Timer();
                {
                    Log("TaskStart {}\n", i);
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                    Log("TaskEnd {}\n", i);
                }
                Log("TaskDuration {}\n", timer.GetDeltaTime());
            });
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(32));
        LogDelimiter("");
         // Timer
        {
            Log("Timer");
            LogDelimiter("");
            Timer timer {};
            Log("Now:          {}", timer.Now());
            Log("Duration:     {}", timer.GetDeltaTime());
            Log("Duration(ns): {}", timer.GetDeltaTimeAs(TimerUnit::Nanoseconds));
            Log("Duration(µs): {}", timer.GetDeltaTimeAs(TimerUnit::Microseconds));
            Log("Duration(ms): {}", timer.GetDeltaTimeAs(TimerUnit::Milliseconds));
            Log("Duration(s):  {}", timer.GetDeltaTimeAs(TimerUnit::Seconds));
            LogDelimiter("");
        }
    }

    ///
    /// @brief  Miscellaneous Tests
    ///
    void Test() {
        logger.Test();
        Log("Default 水 öäü");
        LogFatal("Fatal");
        LogError("Error");
        LogWarning("Warn");
        LogInfo("Info");
        LogDebug("Debug");
        LogTrace("Trace");
    }

    //virtual void OnKeyboardEvent(KeyboardEventData &data, const EventListener::EventEmitter &emitter) override {
    //    return;
    //    if (data.State == KeyState::Release) {
    //        if (data.Key == KeyCode::F9) {
    //            mRenderer->Dispose();
    //            auto api = Context::GetAPI() == GraphicsAPI::OpenGL ? GraphicsAPI::Vulkan : GraphicsAPI::OpenGL;
    //            Context::SetAPI(api);
    //            Reload();
    //            mRenderer = Renderer::Create();
    //        }
    //    }
    //}

private:
    DesignerCamera mDesignerCamera;
    Reference<Texture> mCheckerBoard;
    Scope<Renderer> mRenderer;

    Ultra::Property<size_t> PropertySize { 50, [this](const size_t &value) {
        return value > 1000 ? 1000 : 0;
    }};
};

// Application Entry-Point
Application *CreateApplication() {
    return new App({ "Test", "1280x1024" });
}

}
