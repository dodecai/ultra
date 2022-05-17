// Module
export module Ultra.Application;

import Ultra.Utility.Timer;

export namespace Ultra {

///
/// @brief This is the main class which controls the workflow during runtime.
///
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
        Timer timer = {};
        double delay = {};
        double frames = {};

        // Logic Loop
        while (mRunning) {
            // Calcualte 
            Timestamp deltaTime = timer.GetDeltaTime();
            frames++;
            delay += deltaTime;
            if (delay >= 1.0f) {
                float msPF = 1000.0f / (float)frames;

                frames = 0;
                delay = 0.0f;
            }

            // Update
            Update(deltaTime);
        }

        // Termination
        Destroy();
    }
    
    // This method executes your initialization code.
    virtual void Create() {};
    // This method executes your termination code.
    virtual void Destroy() {}
    // This method executes your main logic code.
    virtual void Update(Timestamp deltatime) {};

    // With this method, everything ends.
    void Exit() {
        mRunning = false;
    }

private:
    // Properties
    bool mRunning = false;
};

// Symbol for Entry-Point
Application *CreateApplication();

}
