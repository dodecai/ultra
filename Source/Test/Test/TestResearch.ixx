export module Ultra.Test.Research;

import Ultra;

export namespace Ultra::Test {

class Research {
public:
    Research() {
        Test();
    }
    ~Research() = default;

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
};

}
