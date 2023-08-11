export module Ultra.Logger;

// Extensions
export import "Private/Logger.h";

// Library
import Ultra.Core;
import Ultra.System.Cli;
import Ultra.Utility.DateTime;
import Ultra.Utility.String;
import <iostream>;
import <fstream>;
import <ranges>;

// Namespace
using std::source_location;
using std::ofstream;
using std::wostream;

// Helpers
namespace Ultra {

consteval size_t constexpr_rfind(const string_view str, char ch, size_t pos = string_view::npos) {
    if (pos >= str.size()) { pos = str.size() - 1; }

    for (size_t i = pos; i != static_cast<size_t>(-1); --i) {
        if (str[i] == ch) { return i; }
    }

    return string_view::npos;
}

consteval string_view ExtractClassFromSignature(const string_view signature) {
    const size_t classEnd = constexpr_rfind(signature, ':') - 1;
    if (classEnd == string_view::npos) { return ""; }

    const size_t namespaceStart = constexpr_rfind(signature, ' ', classEnd) + 1;
    if (namespaceStart == string_view::npos) { return ""; }

    return signature.substr(namespaceStart, classEnd - namespaceStart);
}

}

///
/// @brief This logger fulfils all your basic needs, and accepts nearly everything you throw at it.
/// The motivation behind this was an easy to use, but performant logger, you pay for what you use. It's
/// basically an extension to cout with some gimmicks.
/// @type Singleton
///
export namespace Ultra {

///
/// Common
///

///
/// @brief SourceLocation (with more information)
/// This code is based on the default implementation (source_location), but contains more information.
///
struct SourceLocation {
    [[nodiscard]] constexpr SourceLocation() noexcept = default;

    [[nodiscard]] static consteval SourceLocation Current(
            const string_view file = __builtin_FILE(),
            const string_view function = __builtin_FUNCTION(),
            const string_view signature = __builtin_FUNCSIG(),
            const uint32_t line = __builtin_LINE(),
            const uint32_t column = __builtin_COLUMN()
    ) noexcept {
        SourceLocation result {};
        result.File = file;
        result.Class = ExtractClassFromSignature(signature);
        result.Function = function;
        result.FunctionSignature = signature;
        result.Line = line;
        result.Column = column;
        return result;
    }

    string_view File {};
    string_view Class {};
    string_view Function {};
    string_view FunctionSignature {};
    uint32_t Line {};
    uint32_t Column {};
};

// Enumerations
enum class LogLevel {
    // Log Levels
    Trace   = 0x1,	// Message (Level 1): Everything that could be interesting in the future.
    Debug   = 0x2,	// Message (Level 2): Debugging messages, which can help at parts, where we are unsure if the code will ever fail.
    Info    = 0x3,	// Message (Level 3): Information that is maybe useful for gathering basic system data.
    Warn    = 0x4,	// Message (Level 4): Has no impact on the application itself but should get sometime fixed.
    Error   = 0x5,  // Message (Level 5): Error messages that have no impact on runtime execution.
    Fatal   = 0x6,	// Message (Level 6): Critical messages which will break runtime execution.
    Default = 0x7,	// Message (Level 7): Default output with nothing added

    // Types
    Caption     = 0x8,	// Structure the log by a given caption
    Delimiter   = 0x9,	// Structure the log with a delimiter
};

// Concepts
template <typename T>
concept typename_logmodifier = std::is_same_v<T, LogLevel>;

// Overrides
template <typename_logmodifier T>
inline ostream &operator<<(ostream &os, T level) {
    switch (level) {
        case Ultra::LogLevel::Trace:   { return os << Ultra::Cli::Color::LightMagenta << " [ Trace ] " << Ultra::Cli::Color::White; }
        case Ultra::LogLevel::Debug:   { return os << Ultra::Cli::Color::LightGreen   << " [ Debug ] " << Ultra::Cli::Color::White; }
        case Ultra::LogLevel::Info:    { return os << Ultra::Cli::Color::LightGray    << " [ Info  ] " << Ultra::Cli::Color::White; }
        case Ultra::LogLevel::Warn:    { return os << Ultra::Cli::Color::LightYellow  << " [ Warn  ] " << Ultra::Cli::Color::White; }
        case Ultra::LogLevel::Error:   { return os << Ultra::Cli::Color::LightRed     << " [ Error ] " << Ultra::Cli::Color::White; }
        case Ultra::LogLevel::Fatal:   { return os << Ultra::Cli::Color::Red          << " [ Fatal ] " << Ultra::Cli::Color::White; }
        default:    { return os << ""; }
    }
}

template <typename_logmodifier T>
inline ofstream &operator<<(ofstream &os, T level) {
    switch (level) {
        case Ultra::LogLevel::Trace:   { return os << " [ Trace ] "; }
        case Ultra::LogLevel::Debug:   { return os << " [ Debug ] "; }
        case Ultra::LogLevel::Info:    { return os << " [ Info  ] "; }
        case Ultra::LogLevel::Warn:    { return os << " [ Warn  ] "; }
        case Ultra::LogLevel::Error:   { return os << " [ Error ] "; }
        case Ultra::LogLevel::Fatal:   { return os << " [ Fatal ] "; }
        default:    { return os << ""; }
    }
}

///
/// @brief LogRecord
/// Contains the format, the location, the loglevel and timestamp.
///
struct LogRecord {
    LogRecord(const char *format, const LogLevel &level = LogLevel::Default, const string_view &timestamp = ""/*apptime.GetTimeStamp()*/, const SourceLocation &location = SourceLocation::Current()):
        Format(format),
        Level(level),
        Location(location),
        Timestamp(timestamp) {
    }

    const char *Format = {};
    mutable LogLevel Level = {};
    SourceLocation Location = {};
    string_view Timestamp = {};
};

///
/// Sinks
///

///
/// @brief Logger Interface (Sink)
///
class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void operator()(const LogRecord &record, std::format_args arguments) = 0;
    virtual ILogger &operator<<(const std::format_args arguments) = 0;

    virtual const string &GetName() = 0;
    virtual void SwitchColored() = 0;
};

///
/// @brief Console Logger
///
class ConsoleLogger: public ILogger {
public:
    ConsoleLogger(const string &name = "Console"): mName(name) {}

    void operator()(const LogRecord &record, std::format_args arguments) override {
        string formattedArguments = std::vformat(record.Format, arguments);
        mStream << Cli::Color::Gray << record.Timestamp << Cli::Color::Default << record.Level;
        string message = std::format("{}: {}", record.Location.Class, formattedArguments);

        std::lock_guard<std::mutex> lock(mMutex);
        mStream << message;
    }
    ConsoleLogger &operator<<(const std::format_args arguments) override {
        string_view format = "";
        if (mColored) {
            mColored = false;
            format = "{:c}";
        } else {
            format = "{}";
        }
        auto message = std::vformat(format, arguments);
        //if constexpr (is_wstring_v<arguments>) mWStream << message;
        //else
        mStream << message;
        return *this;
    }

    const string &GetName() override { return mName; }
    void SwitchColored() { mColored = true; }

private:
    string mName {};
    bool mColored {};
    mutex mMutex;
    ostream &mStream = std::cout;
    wostream &mWStream = std::wcout;
};

///
/// @brief File Logger
///
class FileLogger: public ILogger {
public:
    FileLogger(const string &file, const string &name = "File"): mStream(file), mName(name) {
        //throw std::runtime_error("Failed to open log file");
    }

    void operator()(const LogRecord &record, std::format_args arguments) override {
        string formattedArguments = std::vformat(record.Format, arguments);
        mStream << record.Timestamp << record.Level;
        string message = std::format("{}: {}", record.Location.Class, formattedArguments);

        std::lock_guard<std::mutex> lock(mMutex);
        mStream << message;
        mStream.flush();
    }
    FileLogger &operator<<(const std::format_args arguments) override {
        auto message = std::vformat("{}", arguments);
        mStream << message;
        mStream.flush();
        return *this;
    }

    const string &GetName() override { return mName; }
    void SwitchColored() { mColored = !mColored; }

private:
    string mName {};
    bool mColored {};
    mutex mMutex;
    std::ofstream mStream;
};


///
/// @brief Logger
/// 
///
class Logger {
    // Constructors and Deconstructor
    Logger(): mLogLevel(LogLevel::Trace) {}
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    ~Logger() = default;

public:
    Logger &operator=(const Logger &) { return Instance(); }
    Logger &operator=(Logger &&) noexcept { return Instance(); }

    // Get the global instance to the logger (... and yeah thread-safe since C++11 according to researches!)
    static Logger &Instance() {
        static Logger instance;
        return instance;
    }

    // Methods
    void Attach(Reference<ILogger> sink) {
        std::unique_lock<mutex> lock(mMutex);
        mSinks.push_back(std::move(sink));
    }
    void Detach(const string &name) {
        std::unique_lock<mutex> lock(mMutex);
        auto it = std::find_if(mSinks.begin(), mSinks.end(), [&name](Reference<ILogger> sink) {
            return sink->GetName() == name;
        });
        
        if (it != mSinks.end()) {
            mSinks.erase(it);
        }
    }

    // Mutators
    void SetLevel(const LogLevel level) {
        std::unique_lock<mutex> lock(mMutex);
        if (level > LogLevel::Fatal) {
            mLogLevel = LogLevel::Fatal;
        } else {
            mLogLevel = level;
        }
    }

    // Format-Support
    template<typename... Args>
    void operator()(const LogRecord &record, Args &&...args) {
        std::lock_guard<mutex> lock(mMutex);
        try {
            for (auto sink : mSinks) {
                sink->operator()(record, std::make_format_args(std::forward<Args>(args)...));
            }
        } catch (std::exception ex) {
        }
    }

    template<typename... Args>
    void operator()(LogLevel level, const LogRecord &record, Args &&...args) {
        record.Level = level;
        this->operator()(record, std::forward<Args>(args)...);
    }

    // Cli modifier stream
    template<Cli::typename_climodifier T>
    Logger &operator<<(T &&data) {
        if (mSkip) return *this;
        mColored = true;
        *this << data;
        return *this;
    }

    // Log modifier stream
    template <typename_logmodifier T>
    Logger &operator<<(T &&data) {
        if constexpr (std::is_same_v<T, LogLevel>) {
            if (data >= mLogLevel) {
                mSkip = false;
                switch (data) {
                    case LogLevel::Default: {
                        *this << Cli::Color::White;
                        break;
                    }
                    case LogLevel::Caption: {
                        mCaptionActive = true;
                        *this << std::format("{}", LogLevel::Caption);
                        break;
                    }
                    case LogLevel::Delimiter: {
                        *this << std::format("{}", LogLevel::Delimiter);
                        break;
                    }
                    default: {
                        auto timestamp = ""/*apptime.GetTimeStamp()*/;
                        *this << Cli::Color::Gray << timestamp << data;
                        break;
                    }
                }
            } else {
                mSkip = true;
            }
        }
        return *this;
    }

    // Output Stream Template Support (std::endl, ...)
    Logger &operator<<(ostream &(*T)(ostream &)) {
        if (mSkip) return *this;
        mCounter++;

        if (mCaptionActive) {
            mCaptionActive = false;
            *this << std::format("{}", LogLevel::Caption);
        }
        *this << Cli::Style::Reset << Cli::Color::White;

        *this << T;
        return *this;
    }
    
    // Stream-Support
    template<typename T>
    Logger &operator<<(T &&data) {
        if (mSkip) return *this;
        if constexpr (is_string_v<T>) {
            if (String::EndsWith(data, "\n")) {
                mCounter++;

                if (mCaptionActive) {
                    mCaptionActive = false;
                    *this << std::format("{}", LogLevel::Caption);;
                }
                *this << Cli::Style::Reset << Cli::Color::White;
            }
        }

        try {
            for (auto sink : mSinks) {
                if (mColored) {
                    mColored = !mColored;
                    sink->SwitchColored();
                }
                *sink << std::make_format_args(std::forward<T>(data));
            }
        } catch (std::exception ex) {
            // ToDo: How to log if the logger can't log?
            //*this <<  "Internal Error while formatting your log message, please check logger call!\n\t" << ex.what();
        }
        return *this;
    }

    // Show me what you got...
    static void Test() {
        // LogLevels
        Logger::Instance() << LogLevel::Caption << "Logger" << "\n";
        Logger::Instance() << LogLevel::Default << "LogLevels" << "\n";
        Logger::Instance() << LogLevel::Fatal   << "Fatal" << "\n";
        Logger::Instance() << LogLevel::Error   << "Error" << "\n";
        Logger::Instance() << LogLevel::Warn    << "Warn" << "\n";
        Logger::Instance() << LogLevel::Info    << "Info" << "\n";
        Logger::Instance() << LogLevel::Debug   << "Debug" << "\n";
        Logger::Instance() << LogLevel::Trace   << "Trace" << "\n";
        Logger::Instance() << LogLevel::Default << "Default" << "\n";
        Logger::Instance() << "\n";

        // Multi-Color
        Logger::Instance()
            << LogLevel::Default        << "This "
            << Cli::Background::White
            << Cli::Color::Black        << "is "
            << Cli::Background::Default
            << Cli::Color::LightBlue    << "a "
            << Cli::Color::LightRed     << "multi"
            << Cli::Color::LightGray    << "-"
            << Cli::Color::LightGreen   << "color"
            << Cli::Color::LightCyan    << "-"
            << Cli::Color::LightBlue    << "test"
            << Cli::Color::Default  << "!\n";

        // Integer Types
        Logger::Instance() << "Integers[i|l|ll|u|ul|ull]: " << 0 << 0l << 0ll << 0u << 0ul << 0ull << "\n";

        // String based Types
        char Char[] = "Char *";
        const char *ConstChar = "ConstChar *";
        std::string String = "String";

        Logger::Instance() << Cli::Color::LightRed << Char << " "
            << Cli::Color::LightGreen << ConstChar << " "
            << Cli::Color::LightBlue << String << "\n";

        //// Wide-String based Types
        //wchar_t WChar_T[] = L"WChar_T *";
        //const wchar_t *ConstWChar_T = L"ConstWChar_T *";
        //std::wstring WString = L"WString";

        //Logger::Instance() << Cli::Color::LightRed << WChar_T << L" "
        //    << Cli::Color::LightGreen << ConstWChar_T << L" "
        //    << Cli::Color::LightBlue << WString << L"\n";
        //Logger::Instance() << std::endl;

        // Mixed String Types
        //Logger::Instance() << "Mixed String Types " << L"and Wide " << "... not wide " << L"and so on.\n";
    }

private:
    // Properties
    bool mColored = false;
    LogLevel mLogLevel;
    mutex mMutex {};
    vector<Reference<ILogger>> mSinks {};

    // States
    uint64_t mCounter {};
    bool mCaptionActive = false;
    bool mSkip = false;
};

class LoggerOld {
    // Constructors and Deconstructor
    LoggerOld() { SetLevel(mLogLevel); };
    LoggerOld(const LoggerOld &) = delete;
    LoggerOld(LoggerOld &&) = delete;
    ~LoggerOld() = default;

public:
    // Get the global instance to the logger (... and yeah thread-safe since C++11 according to researches!)
    static LoggerOld &Instance() {
        static LoggerOld instance;
        return instance;
    }

    // Operators
    LoggerOld &operator=(const LoggerOld &) { return Instance(); }
    LoggerOld &operator=(LoggerOld &&) noexcept { return Instance(); }

    // Mutators
    void SetLevel(const LogLevel level) {
        std::unique_lock<mutex> lock(mSync);
        if (level > LogLevel::Fatal) {
            mLogLevel = LogLevel::Fatal;
        } else {
            mLogLevel = level;
        }
    }

    // Operators
    template <typename_logmodifier T>
    LoggerOld &operator<<(T &&data) {
        std::unique_lock<mutex> lock(mSync);

        if constexpr (std::is_same_v<T, LogLevel>) {
            if (data >= mLogLevel) {
                mSkip = false;
                auto timestamp = ""/*apptime.GetTimeStamp()*/;
                switch (data) {
                    case LogLevel::Fatal: {
                        mStream << std::format("{}{} {}[{: ^7}] ", Cli::Color::Gray, timestamp, Cli::Color::Red, "Fatal");
                        break;
                    }
                    case LogLevel::Error: {
                        mStream << std::format("{}{} {}[{: ^7}] ", Cli::Color::Gray, timestamp, Cli::Color::LightRed, "Error");
                        break;
                    }
                    case LogLevel::Warn: {
                        mStream << std::format("{}{} {}[{: ^7}] ", Cli::Color::Gray, timestamp, Cli::Color::LightYellow, "Warn");
                        break;
                    }
                    case LogLevel::Info: {
                        mStream << std::format("{}{} {}[{: ^7}] ", Cli::Color::Gray, timestamp, Cli::Color::LightGray, "Info");
                        break;
                    }
                    case LogLevel::Debug: {
                        mStream << std::format("{}{} {}[{: ^7}] ", Cli::Color::Gray, timestamp, Cli::Color::LightGreen, "Debug");
                        break;
                    }
                    case LogLevel::Trace: {
                        mStream << std::format("{}{} {}[{: ^7}] ", Cli::Color::Gray, timestamp, Cli::Color::LightMagenta, "Trace");
                        break;
                    }

                    case LogLevel::Caption: {
                        mCaptionActive = true;
                        constexpr auto seperator = "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
                        mStream << std::format("{}{}  ", Cli::Color::LightBlue, seperator);
                        break;
                    }
                    case LogLevel::Delimiter: {
                        mStream << Cli::Color::Yellow << "----------------------------------------------------------------\n";
                        break;
                    }

                    default: {
                        mStream << Cli::Color::White;
                        break;
                    }
                }
            } else {
                mSkip = true;
            }
        }

        return (*this);
    }

    template<Cli::typename_climodifier T>
    LoggerOld &operator<<(T &&data) {
        std::unique_lock<mutex> lock(mSync);
        if (mSkip) return (*this);
        mStream << std::forward<T>(data);
        return (*this);
    }

    template<typename T>
    LoggerOld &operator<<(T &&data) {
        std::unique_lock<mutex> lock(mSync);
        if (mSkip) return (*this);

        if constexpr (is_string_v<T>) {
            if (String::EndsWith(data, "\n")) {
                if (mCaptionActive) {
                    mCaptionActive = false;
                    mStream << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
                }

                mStream << Cli::Style::Reset << Cli::Color::White;
                mCounter++;
            }
        }

        if constexpr (is_wstring_v<T>) mWStream << std::forward<T>(data);
        else mStream << std::forward<T>(data);
        return (*this);
    }

    LoggerOld &operator<<(ostream &(*T)(ostream &)) {
        std::unique_lock<mutex> lock(mSync);
        if (mSkip) return (*this);

        if (mCaptionActive) {
            mStream << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
            mCaptionActive = false;
        }

        mStream << Cli::Style::Reset << Cli::Color::White;
        mCounter++;

        mStream << T;
        return (*this);
    }

    template <typename ...Args>
    LoggerOld &operator()(const string &format, const Args &...args) {
        //<< location.function_name(), std::source_location location = std::source_location::current
        try {
            return LoggerOld::Instance() << std::vformat(format, std::make_format_args(args...));
        } catch (std::exception ex) {
            return LoggerOld::Instance() << "Internal Error while formatting your log message, please check logger call!\n\t" << ex.what();
        }
    }

private:
    // Properties
    LogLevel mLogLevel = LogLevel::Trace;
    string mLocation;
    ostream &mStream = std::cout;
    wostream &mWStream = std::wcout;
    mutex mSync;

    // States
    size_t mCounter = 0;
    bool mCaptionActive = false;
    bool mSkip = false;
};

inline Logger &ilogger = Logger::Instance();
inline LoggerOld &logger = LoggerOld::Instance();

///
/// @brief  As good as a logger can be, we need something for applications where performance matters. Therefore these function templates are for convenience,
/// they will help removing unnecessary code in release and distribution builds, therefore they also override the log levels.
///

template<typename ...Args> void Log(Args &&...args) { logger << LogLevel::Default; (logger << ... << args); logger << "\n"; }
#ifdef APP_MODE_DEBUG
    // Workaround, add the debug break after the message.
    //#define AppAssert(x, ...)   if (AppAssert(x, __VA_ARGS__)) APP_DEBUGBREAK()
template<typename T, typename ...Args> bool AppAssert(T *object, Args &&...args) {
    if (!object) {
        logger << LogLevel::Fatal; (logger << ... << args); logger << "\n";
        return true;
    }
    return false;
}
template<typename T, typename ...Args> bool AppAssert(T object, Args &&...args) {
    if (!object) {
        logger << LogLevel::Fatal; (logger << ... << args); logger << "\n";
        return true;
    }
    return false;
}
template<typename ...Args> void LogTrace(Args &&...args) { logger << LogLevel::Trace;    (logger(args...)); logger << "\n"; }
template<typename ...Args> void LogDebug(Args &&...args) { logger << LogLevel::Debug;    (logger(args...)); logger << "\n"; }
template<typename ...Args> void LogInfo(Args &&...args) { logger << LogLevel::Info;     (logger(args...)); logger << "\n"; }
#elif APP_MODE_DISTRIBUTION
template<typename ...Args> void AppAssert([[maybe_unused]] Args &&...args) {}
template<typename ...Args> void LogTrace([[maybe_unused]] Args &&...args) {}
template<typename ...Args> void LogDebug([[maybe_unused]] Args &&...args) {}
template<typename ...Args> void LogInfo(Args &&...args) { logger << LogLevel::Info;     (logger(args...)); logger << "\n"; }
#elif APP_MODE_RELEASE
template<typename ...Args> void AppAssert([[maybe_unused]] Args &&...args) {}
template<typename ...Args> void LogTrace([[maybe_unused]] Args &&...args) {}
template<typename ...Args> void LogDebug([[maybe_unused]] Args &&...args) {}
template<typename ...Args> void LogInfo([[maybe_unused]] Args &&...args) {}
#endif
template<typename ...Args> void LogWarning(Args &&...args)  { logger << LogLevel::Warn;     (logger(args...)); logger << "\n"; }
template<typename ...Args> void LogError(Args &&...args)    { logger << LogLevel::Error;    (logger(args...)); logger << "\n"; }
template<typename ...Args> void LogFatal(Args &&...args)    { logger << LogLevel::Fatal;    (logger(args...)); logger << "\n"; }

}

// Global Specialization
template <>
struct std::formatter<Ultra::LogLevel> {
    constexpr auto parse(std::format_parse_context &ctx) {
        if (auto it = std::ranges::find(ctx.begin(), ctx.end(), 'c'); it != ctx.end()) {
            mColored = true;
            return it;
        }
        return ctx.begin();
    }

    auto format(const Ultra::LogLevel &level, std::format_context &ctx) const {
        if (mColored) {
            switch (level) {
                case Ultra::LogLevel::Trace:   { return format_to(ctx.out(), " {}[ Trace ]{} ", Ultra::Cli::Color::LightMagenta, Ultra::Cli::Color::White); }
                case Ultra::LogLevel::Debug:   { return format_to(ctx.out(), " {}[ Debug ]{} ", Ultra::Cli::Color::LightGreen  , Ultra::Cli::Color::White); }
                case Ultra::LogLevel::Info:    { return format_to(ctx.out(), " {}[ Info  ]{} ", Ultra::Cli::Color::LightGray   , Ultra::Cli::Color::White); }
                case Ultra::LogLevel::Warn:    { return format_to(ctx.out(), " {}[ Warn  ]{} ", Ultra::Cli::Color::LightYellow , Ultra::Cli::Color::White); }
                case Ultra::LogLevel::Error:   { return format_to(ctx.out(), " {}[ Error ]{} ", Ultra::Cli::Color::LightRed    , Ultra::Cli::Color::White); }
                case Ultra::LogLevel::Fatal:   { return format_to(ctx.out(), " {}[ Fatal ]{} ", Ultra::Cli::Color::Red         , Ultra::Cli::Color::White); }

                case Ultra::LogLevel::Caption: {
                    return format_to(ctx.out(),
                        "{}\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n{}",
                        Ultra::Cli::Color::LightBlue,
                        Ultra::Cli::Color::White
                    );
                }
                case Ultra::LogLevel::Delimiter: {
                    return format_to(ctx.out(),
                        "{}----------------------------------------------------------------\n{}",
                        Ultra::Cli::Color::Yellow,
                        Ultra::Cli::Color::White
                    );
                }
            }
        } else {
            switch (level) {
                case Ultra::LogLevel::Trace:   { return format_to(ctx.out(), " [ Trace ] "); }
                case Ultra::LogLevel::Debug:   { return format_to(ctx.out(), " [ Debug ] "); }
                case Ultra::LogLevel::Info:    { return format_to(ctx.out(), " [ Info  ] "); }
                case Ultra::LogLevel::Warn:    { return format_to(ctx.out(), " [ Warn  ] "); }
                case Ultra::LogLevel::Error:   { return format_to(ctx.out(), " [ Error ] "); }
                case Ultra::LogLevel::Fatal:   { return format_to(ctx.out(), " [ Fatal ] "); }

                case Ultra::LogLevel::Caption: {
                    return format_to(ctx.out(), "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n");
                }
                case Ultra::LogLevel::Delimiter: {
                    return format_to(ctx.out(), "----------------------------------------------------------------\n");
                }
            }
        }
    }

private:
    bool mColored = false;
};

