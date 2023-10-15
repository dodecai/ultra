export module Ultra.Debug.Profiler;

import Ultra.Core;
import Ultra.Logger;


///
/// @brief: Profiles code and generates tracing data for chrome based browsers (URL: chrome://tracing/)
///

namespace Ultra::Debug {

// Types
using SteadyClock = std::chrono::steady_clock;
using Duration = std::chrono::microseconds;
using TimeStep = std::chrono::duration<double, std::micro>;

// ToDo: Hold all results in memory, later for the UI
struct ProfilerResult {
    string Name {};
    std::thread::id ThreadID {};
    double StartTime {};
    double DeltaTime {};
} static sProfilerResults {};

using ProfileResults = std::vector<ProfilerResult>;

struct ProfilerSession {
    string Name;
};

///
/// @brief: Instrumentor
/// 
/// @example
/// ProfileScope("Application");
///
class Instrumentor {
private:
    Instrumentor() = default;
    ~Instrumentor() = default;

public:
    static Instrumentor &Instance() {
        static Instrumentor instance;
        return instance;
    }

    void BeginSession(const std::string &name, const std::string &file = "results.json") {
        std::lock_guard lock(mMutex);
        if (mCurrentSession) { EndSessionInternal(); }
        mStream.open(file);

        if (mStream.is_open()) {
            mCurrentSession = CreateScope<ProfilerSession>(name);
            WriteHeader();
        } else {
            Ultra::LogError("Instrumentor couldn't open target file '{}'!\n", file);
        }
    }
    void WriteProfile(const ProfilerResult &result) {
        std::lock_guard lock(mMutex);

        stringstream json;
        json << std::setprecision(3) << std::fixed;
        json <<
            ",\n"
            << "    {"
            << R"( "name": ")"   << result.Name         << R"(", )"
            << R"( "cat": ")"    << "function"          << R"(", )"
            << R"( "ph": ")"     << "X"                 << R"(", )"
            << R"( "ts": )"      << result.StartTime    << R"(, )"
            << R"( "dur": )"     << result.DeltaTime    << R"(, )"
            << R"( "pid": )"     << 0                   << R"(, )"
            << R"( "tid": )"     << result.ThreadID     << R"(, )"
            << R"( "args": )"    << "{}"                << R"( )"
            << " }";

        if (mCurrentSession) {
            mStream << json.str();
            mStream.flush();
        }
    }
    void EndSession() {
        std::lock_guard lock(mMutex);
        EndSessionInternal();
    }

private:
    void EndSessionInternal() {
        if (mCurrentSession) {
            WriteFooter();
            mStream.close();
            mCurrentSession.reset(nullptr);
        }
    }

    void WriteHeader() {
        auto application = "Ultra";
        auto version = "v1.0.0";
        mStream << "{\n"
            << R"(  "displayTimeUnit": "ms",)"  << "\n"
            << R"(  "otherData": {)"            << "\n"
            << R"(    "application": ")"    << application  << R"(",)"  << "\n"
            << R"(    "version": ")"        << version      << R"(")"   << "\n"
            << R"(  },)"                    << "\n"
            << R"(  "traceEvents": [)"      << "\n"
            << R"(    {})";
        mStream.flush();
    }
    void WriteFooter() {
        mStream
            << "\n"
            << "  ]\n"
            << "}\n";
        mStream.flush();
    }

private:
    Scope<ProfilerSession> mCurrentSession {};
    mutex mMutex {};
    ofstream mStream {};
};

///
/// @brief: Internal scoped timer
///
class ScopedTimer {
public:
    ScopedTimer(const char *name, const function<void(const string &, double duration)> &function = nullptr):
        mName(name),
        mStopped(false),
        mStartTime(SteadyClock::now()),
        mCallback(function) {
    }
    ~ScopedTimer() { if (!mStopped) { Stop(); } };

    void Stop() {
        auto stopTime = std::chrono::time_point_cast<Duration>(SteadyClock::now()).time_since_epoch().count();
        auto startTime = std::chrono::time_point_cast<Duration>(mStartTime).time_since_epoch().count();
        auto duration = (stopTime - startTime) * 0.001f;

        mStopped = true;
        if (mCallback) {
            mCallback(mName, duration);
        } else {
            ProfilerResult result { mName, std::this_thread::get_id(), static_cast<double>(startTime), duration };
            Instrumentor::Instance().WriteProfile(result);
        }
    }

private:
    const char *mName;
    bool mStopped {};
    std::chrono::time_point<SteadyClock> mStartTime;
    function<void(const string&, double duration)> mCallback;
};

}

export namespace Ultra::Debug {

void StartProfiling(const string &name, const string &file = "ProfilerResults.json") {
    Instrumentor::Instance().BeginSession(name, file);
}

ScopedTimer ProfileScope(const char *name, const function<void(const string &, double duration)> &function = nullptr, [[maybe_unused]] const SourceLocation &location = SourceLocation::Current()) {
    return ScopedTimer(name, function);
}

void StopProfiling() {
    Instrumentor::Instance().EndSession();
}

}
