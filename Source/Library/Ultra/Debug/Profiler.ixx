module;

export module Ultra.Debug.Profiler;

import <algorithm>;
import <chrono>;
import <fstream>;
import <mutex>;
import <iomanip>;
import <string>;
import <sstream>;
import <thread>;

import Ultra.Logger;

///
/// @brief: Profile your code and generate tracing data for chrome based browsers (URL: chrome://tracing/)
/// 
/// Example:
/// PROFILE_SCOPE("TestLayer2D::Update");
/// ...
/// ImGui::Begin("Statistics");
/// for (auto &result : ProfilerResults) {
/// 	char label[64];
/// 	strcpy(label, "%.3fms ");
/// 	strcat(label, result.Name);
/// 	ImGui::Text(label, result.Time);
/// }
/// ProfilerResults.clear();
/// 

export namespace Ultra {

namespace Profiler {

// Properties
using TimeStep = std::chrono::duration<double, std::micro>;

struct Result {
    std::string Name;
    std::thread::id ThreadID;

    TimeStep StartTime;
    std::chrono::microseconds DeltaTime;
};

struct Session {
    std::string Name;
};


class Instrumentor {
public:
    Instrumentor(): m_CurrentSession(nullptr) {}

    static Instrumentor &Get() {
        static Instrumentor instance;
        return instance;
    }

    void BeginSession(const std::string &name, const std::string &filepath = "results.json") {
        std::lock_guard lock(m_Mutex);
        if (m_CurrentSession) { InternalEndSession(); }
        m_OutputStream.open(filepath);

        if (m_OutputStream.is_open()) {
            m_CurrentSession = new Session({ name });
            WriteHeader();
        } else {
            Ultra::logger << "Instrumentor couldn't open target file!\n";
        }
    }

    void EndSession() {
        std::lock_guard lock(m_Mutex);
        InternalEndSession();
    }

    void WriteProfile(const Result &result) {
        std::lock_guard lock(m_Mutex);
        std::stringstream json;

        json << std::setprecision(3) << std::fixed;
        json << ",\n    {";
        json << R"("name": ")" << result.Name << R"(", )";
        json << R"("cat": ")" << "function" << R"(", )";
        json << R"("ph": ")" << "X" << R"(", )";
        json << R"("ts": )" << result.StartTime.count() << R"(, )";
        json << R"("dur": )" << (result.DeltaTime.count()) << R"(, )";
        json << R"("pid": )" << 0 << R"(, )";
        json << R"("tid": )" << result.ThreadID << R"(, )";
        json << R"("args": )" << "{}" << R"( )";
        json << "}";

        if (m_CurrentSession) {
            m_OutputStream << json.str();
            m_OutputStream.flush();
        }
    }

private:
    void WriteHeader() {
        auto application = "Playground";
        auto version = "v1.0.0";
        m_OutputStream << "{\n";
        m_OutputStream << R"(  "displayTimeUnit": ")" << "ms" << R"(",)" << "\n";
        m_OutputStream << R"(  "otherData": {)" << "\n";
        m_OutputStream << R"(    "application": ")" << application << R"(",)" << "\n";
        m_OutputStream << R"(    "version": ")" << version << R"(")" << "\n";
        m_OutputStream << R"(  },)" << "\n";
        m_OutputStream << R"(  "traceEvents": [)" << "\n";
        m_OutputStream << R"(    {})";
        m_OutputStream.flush();
    }

    void WriteFooter() {
        m_OutputStream << "\n  ]";
        m_OutputStream << "\n}\n";
        m_OutputStream.flush();
    }

    // Note: you must already own lock on m_Mutex before  calling InternalEndSession()
    void InternalEndSession() {
        if (m_CurrentSession) {
            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
        }
    }

private:
    std::mutex m_Mutex;
    Session *m_CurrentSession;
    std::ofstream m_OutputStream;
};

class Timer {
    const char *m_Name;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    bool m_Stopped;

public:
    Timer(const char *name):
        m_Name(name), m_Stopped(false) {
        m_StartTimepoint = std::chrono::steady_clock::now();
    }

    ~Timer() {
        if (!m_Stopped) Stop();
    }

    void Stop() {
        auto endTimepoint = std::chrono::steady_clock::now();
        auto highResStart = TimeStep { m_StartTimepoint.time_since_epoch() };
        auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

        Instrumentor::Get().WriteProfile({ m_Name, std::this_thread::get_id(), highResStart, elapsedTime });

        m_Stopped = true;
    }
};

namespace Utilities {

template <size_t N>
struct ChangeResult {
    char Data[N];
};

template <size_t N, size_t K>
constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K]) {
    ChangeResult<N> result = {};

    size_t srcIndex = 0;
    size_t dstIndex = 0;
    while (srcIndex < N) {
        size_t matchIndex = 0;
        while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
            matchIndex++;
        if (matchIndex == K - 1)
            srcIndex += matchIndex;
        result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
        srcIndex++;
    }
    return result;
}

}

}

}

///
/// @brief: Internal scoped timer
///
export namespace Ultra {

struct ProfilerResult {
    const char *Name;
    float Time;
};

std::vector<ProfilerResult> ProfileResults;

template <typename L>
class ScopedTimer {
public:
    ScopedTimer(const char *name, L &&function):
        Name { name },
        Stopped { false },
        Function { function } {
        StartTimePoint = std::chrono::high_resolution_clock::now();
    }
    ~ScopedTimer() { if (!Stopped) { Stop(); } };

    void Stop() {
        auto stopTimePoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTimePoint).time_since_epoch().count();
        auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(stopTimePoint).time_since_epoch().count();

        Stopped = true;

        auto duration = (stop - start) * 0.001f;
        Function({ Name, duration });
    }

private:
    bool Stopped;
    const char *Name;
    std::chrono::time_point<std::chrono::steady_clock> StartTimePoint;
    L Function;
};

}
