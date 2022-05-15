export module Ultra.Application;

export namespace Ultra {

// Application
class Application {
public:
    // Constructors and Destructor
    Application():
        mRunning(true) {
    };
    virtual ~Application() = default;

    // With this method, everything begins.
    void Run() {
        // Initialization
        Create();

        // Logic Loop
        while (mRunning) {
            Update();
        }

        // Termination
        Destroy();
    }
    
    // These methods offer an easy to use applicaiton workflow.
    /// This method executes your initialization code.
    virtual void Create() {};
    /// This method executes your termination code.
    virtual void Destroy() {}
    /// This method executes your main logic code.;
    virtual void Update() {};

    // With this method, everything ends.
    void Exit() {
        mRunning = false;
    }

private:
    // Properties
    bool mRunning = false;
};

// Symbol for Main
Application *CreateApplication();

}
