#include <Settings.h>
#include <Ultra/EntryPoint.h>

import Ultra;
import Ultra.Engine.DesignerCamera;
import Ultra.Utility.ThreadPool;

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

#pragma endregion

// Application
class App: public Application {
    struct EventEmitter: Emitter<EventEmitter> {};

public:
    // Constructors and Destructor
    App(const ApplicationProperties &properties): Application(properties) {}
    ~App() = default;

    // Methods
    void Create() {
        #ifdef ENGINE_TESTS
            mRenderer = Renderer::Create();
            auto swapchain = Swapchain::Create(nullptr, 1280, 720);

            auto aspectRatio = 800.0f / 600.0f;
            mDesignerCamera = DesignerCamera(45.0f, aspectRatio, 0.1f, 1000.0f);

            auto commandBuffer = CommandBuffer::Create();
            mCheckerBoard = Texture::Create(TextureProperties(), "./Assets/Textures/CheckerBoard.png");

            // Load shaders, buffers, textures
            auto linkedShaders = Shader::Create("Assets/Shaders/Test.glsl");
            //auto linkedShaders = Shader::Create("Assets/Shaders/Sample.glsl");
            //auto vertexBuffer = Buffer::Create(BufferType::VertexBuffer, vertices, vertexCount);
            //auto indexBuffer = Buffer::Create(BufferType::IndexBuffer, indices, indexCount);
            //auto texture = Texture::Create(TextureType::Texture2D, "path/to/texture.png");

            //// Create render states
            //auto renderState = RenderState::Create();
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
        // Begin recording commands
        //commandBuffer->Begin();
        //commandBuffer->Clear(0.2f, 0.3f, 0.3f, 1.0f);     // Clear the framebuffer
        //commandBuffer->BindRenderState(renderState);      // Set up the render state

        mRenderer->RenderFrame();
        mDesignerCamera.Update(deltaTime);
        Renderer2D::StartScene(mDesignerCamera);

        // Bind shaders, buffers, textures
        //    commandBuffer->BindShader(vertexShader);
        //    commandBuffer->BindShader(fragmentShader);
        //    commandBuffer->BindVertexBuffer(vertexBuffer);
        //    commandBuffer->BindIndexBuffer(indexBuffer);
        //    commandBuffer->BindTexture(0, texture);
        //    commandBuffer->DrawIndexed(indexCount);           // Draw the mesh
         
        
        // 3D Renderer: Primitives
        //mRenderer->Test();

        // 2D Renderer: Primitives
        Renderer2D::DrawLine({ -0.9f, -0.9f }, {  0.9f,  -0.9f }, { 0.9f, 0.9f, 0.9f, 1.0f });
        Renderer2D::DrawLine({ -0.9f, -0.9f }, { -0.9f,   0.9f }, { 1.0f, 0.0f, 1.0f, 1.0f });
        Renderer2D::DrawLine({  0.2f,  0.2f }, {  0.7f,   0.7f }, { 1.0f, 1.0f, 1.0f, 1.0f });
        Renderer2D::DrawQuad({ -0.6f, -0.6f }, {  0.5f,   0.5f }, mCheckerBoard, 1.0f, { 1.0f, 0.0f, 0.0f, 1.0f });
        Renderer2D::DrawQuad({  0.2f,  0.2f }, {  0.7f,   0.7f }, mCheckerBoard, 1.0f, { 0.0f, 0.0f, 1.0f, 1.0f });
        Renderer2D::DrawRect({ -0.9f,  0.9f }, {  0.5f,   0.5f }, { 0.2f, 0.2f, 0.2f, 1.0f });
        Renderer2D::DrawCircle({ 1.0f, 1.0f }, {  0.5f,   0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, 0.1f, 0.1f);

        static float rotation = 0.0f;
        const float speed = 180.0f;
        rotation += speed * deltaTime;
        if (rotation >= 360.0f) rotation = 0.0f;
        Renderer2D::DrawRotatedQuad({  0.7f,   0.7f }, { 0.2f,  0.2f }, rotation * (3.14f / 180.0f), mCheckerBoard, 1.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
        Renderer2D::DrawRotatedQuad({  0.7f,  -0.6f }, { 0.4f,  0.4f }, rotation * (3.14f / 180.0f) * -1.0f, mCheckerBoard, 1.0f, { 0.7f, 0.7f, 0.7f, 1.0f });

        // Finish
        Renderer2D::FinishScene();
        mRenderer->DrawGrid(mDesignerCamera);
        //Renderer::EndScene();
     
        //    commandBuffer->End();                             // End recording commands
        //    commandBuffer->Execute();                         // Execute the command buffer
        //    swapchain->Present();                             // Present the rendered image to the screen
    }

    ///
    /// @brief Library Tests
    ///
    void LibraryTest() {
        ///
        /// Core
        ///
        logger << LogLevel::Caption << "Core Library" << "\n";
        // Logger
        ilogger.Attach(CreateReference<ConsoleLogger>());
        ilogger.Attach(CreateReference<FileLogger>("Test.log"));
        ilogger.Test();
        ilogger(LogLevel::Warn, "Test {}, if you can{}!\n", "me", 2);
        ilogger(LogLevel::Error, "Test me, if you can{}!\n", 3);

        ///
        /// Utilities
        ///
        logger << LogLevel::Caption << "Utilities" << "\n";
        // DateTime
        logger << "DateTime" << "\n";
        logger << LogLevel::Delimiter;
        logger(" - CurrentDate:      {}\n", apptime.GetDate());
        logger(" - CurrentTime:      {}\n", apptime.GetTime());
        logger(" - CurrentTimestamp: {}\n", apptime.GetTimeStamp());
        logger(" - CurrentRuntime:   {}\n", apptime.GetRuntime());
        logger << LogLevel::Delimiter;

        // Logger
        logger << LogLevel::Trace << "Hello World! 🦄" << "\n";
        LogTrace("{}: {} {:.2}", "Hello", "World!", 1.234567f);

        // Property
        //LogTrace("PropertySize before: ", PropertySize);
        PropertySize = 1001;
        //LogTrace("PropertySize after setting: ", PropertySize);

        // String
        String::Test();

        // ThreadPool
        ThreadPool pool;
        // Create eight tasks in the queue
        for (int i = 0; i < 8; ++i) {
            pool.Enqueue([i] {
                logger << "TaskStart " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                logger << "TaskEnd " << i << std::endl;
            });
        }

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

    ///
    /// @brief  Miscellaneous Tests
    ///
    void Test() {
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
    return new App({ "Spectra", "1280x1024" });
}

}
