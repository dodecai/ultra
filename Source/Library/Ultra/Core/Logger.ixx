export module Ultra.Logger;

// Library
import Ultra.Core;
import Ultra.Core.DateTime;
import Ultra.Core.String;
import Ultra.System.Cli;

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
/// @brief Contains the classic Log Levels with extensions for structuring
/// Messaging: Trace, Debug, Info, Warn, Error, Fatal + Default (simple message)
/// Structuring: Caption and Delimiter
///
enum class LogLevel: uint32_t {
    // Log Levels
    Trace   = 0x1,	// Message (Level 1): Everything that could be interesting in the future.
    Debug   = 0x2,	// Message (Level 2): Debugging messages, which can help at parts, where we are unsure if the code will ever fail.
    Info    = 0x3,	// Message (Level 3): Information that is maybe useful for gathering basic system data.
    Warn    = 0x4,	// Message (Level 4): Has no impact on the application itself but should get sometime fixed.
    Error   = 0x5,  // Message (Level 5): Error messages that have no impact on runtime execution.
    Fatal   = 0x6,	// Message (Level 6): Critical messages which will break runtime execution.
    Default = 0x7,	// Message (Level 7): Default output with nothing added

    // Structuring
    Caption     = 0x20,	// Structure the log by a given caption
    Delimiter   = 0x21,	// Structure the log with a delimiter
};

// Concepts
template <typename T>
concept typename_logmodifier =
    std::is_same_v<std::decay_t<T>, LogLevel>;

// Overloads
template <typename_logmodifier T>
inline auto &operator<<(ostream &os, const T &level) {
    switch (level) {
        case Ultra::LogLevel::Trace:   { return os << " [ Trace ] "; }
        case Ultra::LogLevel::Debug:   { return os << " [ Debug ] "; }
        case Ultra::LogLevel::Info:    { return os << " [ Info  ] "; }
        case Ultra::LogLevel::Warn:    { return os << " [ Warn  ] "; }
        case Ultra::LogLevel::Error:   { return os << " [ Error ] "; }
        case Ultra::LogLevel::Fatal:   { return os << " [ Fatal ] "; }

        case Ultra::LogLevel::Caption: {
            return os << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
        }
        case Ultra::LogLevel::Delimiter: {
            return os << "----------------------------------------------------------------\n";
        }

        default: { return os; }
    }
}

template <typename_logmodifier T>
inline auto &operator<<(wostream &os, const T &level) {
    switch (level) {
        case Ultra::LogLevel::Trace:   { return os << " [ Trace ] "; }
        case Ultra::LogLevel::Debug:   { return os << " [ Debug ] "; }
        case Ultra::LogLevel::Info:    { return os << " [ Info  ] "; }
        case Ultra::LogLevel::Warn:    { return os << " [ Warn  ] "; }
        case Ultra::LogLevel::Error:   { return os << " [ Error ] "; }
        case Ultra::LogLevel::Fatal:   { return os << " [ Fatal ] "; }

        case Ultra::LogLevel::Caption: {
            return os << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
        }
        case Ultra::LogLevel::Delimiter: {
            return os << "----------------------------------------------------------------\n";
        }

        default: { return os; }
    }
}

///
/// @brief LogRecord
/// Contains the format, the location, the loglevel and timestamp.
/// 
/// @remark I tried Timestamp as string_view, it worked ... but only as function parameter,
/// when I tried to create LogRecord normally, the Timestamp was corrupted, so as GetTimeStamp()
/// creates a string anyway, it should be fine to leave it as string. After some tests, it was
/// faster then string_view, 'cause under the hood it created a temporary string_view.
///
struct LogRecord {
    const char *Format;
    mutable LogLevel Level;
    SourceLocation Location;
    string Timestamp;

    LogRecord(const char *format, const LogLevel &level = LogLevel::Default, const string &timestamp = apptime.GetTimeStamp(), const SourceLocation &location = SourceLocation::Current()):
        Format(format),
        Level(level),
        Location(location),
        Timestamp(timestamp) {
    }

    bool operator==(const LogRecord &other) const {
        return Level == other.Level && Timestamp == other.Timestamp;
    }
};

struct LogRecordHasher {
    size_t operator()(const LogRecord &record) const {
        return std::hash<LogLevel>{}(record.Level) ^ std::hash<string>{}(record.Timestamp);
    }
};

///
/// Sinks
///

enum class LoggerType {
    Console = 0x0,
    File    = 0x1,
    Memory  = 0x2,
};

///
/// @brief Logger Sink Interface
///
class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void operator()(const LogRecord &record, std::format_args arguments) = 0;

    virtual const string &GetName() const = 0;
    virtual LoggerType GetType() const = 0;
};

///
/// @brief Console Sink
///
class ConsoleLogger: public ILogger {
public:
    ConsoleLogger(const string &name = "Console"): mName(name) {}

    void operator()(const LogRecord &record, std::format_args arguments) override {
        std::lock_guard<std::mutex> lock(mMutex);

        auto levelColor = GetLogLevelColor(record.Level);
        string message = std::vformat(record.Format, arguments);
        // mStream << std::format...
        switch (record.Level) {
            case LogLevel::Default: {
                mStream << std::format("{}{}{}",
                    levelColor, message,
                    Cli::Style::Reset
                );
                break;
            }
            case LogLevel::Caption: {
                mStream << std::format("{}{}  {}{}{}{}{}",
                    levelColor, record.Level,
                    Cli::Color::White, message,
                    levelColor, record.Level,
                    Cli::Style::Reset, Cli::Color::Default
                );
                break;
            }
            case LogLevel::Delimiter: {
                mStream << std::format("{}{}{}{}",
                    levelColor, record.Level,
                    Cli::Style::Reset, Cli::Color::Default
                );
                break;
            }
            default: {
                mStream << std::format("{}{}{}{}{}<{}> {}{}{}{}",
                    Cli::Color::Gray, record.Timestamp,
                    levelColor, record.Level,
                    Cli::Color::LightGray, record.Location.Class,
                    Cli::Color::White, message,
                    Cli::Style::Reset, Cli::Color::Default
                );
                break;
            }
        }
    }

    const string &GetName() const override { return mName; }
    LoggerType GetType() const override { return mType; }

private: 
    Cli::Color GetLogLevelColor(LogLevel level) {
        switch (level) {
            case LogLevel::Trace:        { return Ultra::Cli::Color::LightMagenta; }
            case LogLevel::Debug:        { return Ultra::Cli::Color::LightGreen; }
            case LogLevel::Info:         { return Ultra::Cli::Color::LightGray; }
            case LogLevel::Warn:         { return Ultra::Cli::Color::LightYellow; }
            case LogLevel::Error:        { return Ultra::Cli::Color::LightRed; }
            case LogLevel::Fatal:        { return Ultra::Cli::Color::Red; }

            case LogLevel::Caption:      { return Ultra::Cli::Color::LightBlue; }
            case LogLevel::Delimiter:    { return Ultra::Cli::Color::Yellow; }

            default:                     { return  Ultra::Cli::Color::White; }
        }
    }

private:
    string mName {};
    LoggerType mType { LoggerType::Console };

    mutex mMutex;
    ostream &mStream = std::cout;
    wostream &mWStream = std::wcout;
};

///
/// @brief File Sink
///
class FileLogger: public ILogger {
public:
    FileLogger(const string &file, const string &name = "File"): mStream(file), mName(name) {
        //throw std::runtime_error("Failed to open log file");
    }

    void operator()(const LogRecord &record, std::format_args arguments) override {
        std::lock_guard<std::mutex> lock(mMutex);

        string message = std::vformat(record.Format, arguments);
        switch (record.Level) {
            case LogLevel::Default: {
                mStream << std::format("{}{}",
                    record.Level,
                    message
                );
                break;
            }
            case LogLevel::Caption: {
                mStream << std::format("{}  {}{}",
                    record.Level,
                    message,
                    record.Level
                );
                break;
            }
            case LogLevel::Delimiter: {
                mStream << std::format("{}",
                    record.Level
                );
                break;
            }
            default: {
                mStream << std::format("{}{}<{}> {}",
                    record.Timestamp,
                    record.Level,
                    record.Location.Class,
                    message
                );
                break;
            }
        }

        mStream.flush();
    }

    const string &GetName() const override { return mName; }
    LoggerType GetType() const override { return mType; }

private:
    string mName {};
    LoggerType mType { LoggerType::File };

    mutex mMutex;
    ofstream mStream;
};

///
/// @brief Memory Sink
///
class MemoryLogger: public ILogger {
private:
    using Messages = unordered_map<LogRecord, string, LogRecordHasher>;

public:
    MemoryLogger(const string &name = "Memory"): mName(name) {}

    void operator()(const LogRecord &record, std::format_args arguments) override {
        std::lock_guard<mutex> lock(mMutex);
        string buffer {};
        std::format_to(std::back_inserter(buffer), "{}", std::vformat(record.Format, arguments));
        mMessages.emplace(record, buffer);
    }

    const string &GetName() const override { return mName; }
    LoggerType GetType() const override { return mType; }
    const Messages &GetMessages() const { return mMessages; }

    void Clear() {
        std::lock_guard<mutex> lock(mMutex);
        mMessages.clear();
    }

private:
    string mName {};
    LoggerType mType { LoggerType::Memory };

    mutex mMutex;
    Messages mMessages;
};


///
/// @brief Logger
/// 
/// @todo WideString Support
/// if constexpr (is_wstring_v<T>) mWStream << std::forward<T>(data);
/// else mStream << std::forward<T>(data);
///
class Logger: public SteadyObject {
private:
    // Constructors and Deconstructor
    Logger(): mLogLevel(LogLevel::Trace) {}
    virtual ~Logger() = default;

public:
    // Get the global instance to the logger
    static Logger &Instance() {
        static Logger instance;
        return instance;
    }

    // Methods
    void Attach(Scope<ILogger> sink) {
        std::unique_lock<mutex> lock(mMutex);
        mSinks.push_back(std::move(sink));
    }
    void Detach(const string &name) {
        std::unique_lock<mutex> lock(mMutex);
        mSinks.erase(std::remove_if(mSinks.begin(), mSinks.end(), [&name](const Scope<ILogger> &sink) {
            return sink->GetName() == name;
        }), mSinks.end());
    }
    void Flush() {
        this->operator()({ "{}", mStreamLogLevel }, mStreamBuffer.view());
        mStreamBuffer.str("");
        mStreamLogLevel = LogLevel::Default;
    }

    // Accessors
    LogLevel GetLevel() const {
        return mLogLevel;
    }

    // Mutators
    void SetLevel(const LogLevel &level) {
        std::unique_lock<mutex> lock(mMutex);
        mLogLevel = level > LogLevel::Fatal ? LogLevel::Fatal : level;
    }

    // Format-Support
    template<typename... Args>
    void operator()(const LogLevel &level, const LogRecord &record, Args &&...args) {
        if (record.Level < mLogLevel) return;
        record.Level = level;
        this->operator()(record, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void operator()(const LoggerType &type, const LogRecord &record, Args &&...args) {
        if (record.Level < mLogLevel) return;
        std::lock_guard<mutex> lock(mMutex);
        mCounter++;
        try {
            auto formattedArgs = std::make_format_args(std::forward<Args>(args)...);
            for (const auto &sink : mSinks) {
                if (sink->GetType() == type) sink->operator()(record, formattedArgs);
            }
        } catch (std::exception ex) {
            //this->operator()("Ultra::Logger: {}", ex.what());
        }
    }

    template<typename... Args>
    void operator()(const LogRecord &record, Args &&...args) {
        if (record.Level < mLogLevel) return;
        std::lock_guard<mutex> lock(mMutex);
        mCounter++;
        try {
            auto formattedArgs = std::make_format_args(std::forward<Args>(args)...);
            for (const auto &sink : mSinks) {
                sink->operator()(record, formattedArgs);
            }
        } catch (std::exception ex) {
            //this->operator()("Ultra::Logger: {}", ex.what());
        }
    }

    // Log modifier stream
    template <typename_logmodifier T>
    Logger &operator<<(T &&data) {
        if constexpr (std::is_same_v<std::decay_t<T>, LogLevel>) {
            if (data >= mLogLevel) {
                mStreamLogLevel = data;
            } else {
                mSkip = true;
            }
        }
        return *this;
    }

    // Output Stream Template Support (std::endl, ...)
    Logger &operator<<([[maybe_unused]] ostream &(*T)(ostream &)) {
        if (mSkip) return *this;

        mStreamBuffer << '\n';
        this->operator()({ "{}", mStreamLogLevel }, mStreamBuffer.view());
        mStreamBuffer.str("");
        mStreamLogLevel = LogLevel::Default;
        return *this;
    }
    
    // Stream-Support
    template<typename T>
    Logger &operator<<(T &&data) {
        if (mSkip) return *this;

        mStreamBuffer << data;
        if constexpr (is_xstring_v<std::decay_t<T>>) {
            if (String::EndsWith(data, "\n")) {
                this->operator()({ "{}", mStreamLogLevel }, mStreamBuffer.view());
                mStreamBuffer.str("");
                mStreamLogLevel = LogLevel::Default;
            }
        }
        return *this;
    }

    // Show me what you got...
    static void Test() {
        // Preparation
        auto &logger = Logger::Instance();

        // LogLevels
        logger(LogLevel::Caption,   "Caption");
        logger(LogLevel::Default,   "Default\n");
        logger(LogLevel::Fatal,     "Fatal\n");
        logger(LogLevel::Error,     "Error\n");
        logger(LogLevel::Warn,      "Warn\n");
        logger(LogLevel::Info,      "Info\n");
        logger(LogLevel::Debug,     "Debug\n");
        logger(LogLevel::Trace,     "Trace\n");
        logger(LogLevel::Delimiter, "");

        // Formatting
        logger("Test {}, if you can {}!\n", "me", 1);
        logger(LogLevel::Warn, "Test {}, if you can {}!\n", "me", 2);
        logger(LogLevel::Error, "Test {}, if you can {}!\n", "me", 3);

        logger << "Test " << "me" << ", if you can " << 4 << "!" << std::endl;
        logger << LogLevel::Warn << "Test " << "me" << ", if you can " << 5 << "!" << "\n";
        logger << LogLevel::Error << "Test " << "me" << ", if you can " << 6 << "!\n";
        logger << LogLevel::Error << "Test " << "me" << ", if you can " << 7 << "!";
        logger.Flush();
        logger << "\n";
        logger(LogLevel::Delimiter, "");

        // Filtered Logging and Multi-Color test (console only)
        logger(LoggerType::Console, "{}{}This {}{}is {}a {}multi{}-{}color{}-{}test!\n",
            Cli::Background::White, Cli::Color::Black,
            Cli::Background::Default, Cli::Color::LightBlue,
            Cli::Color::LightRed,
            Cli::Color::LightGray,
            Cli::Color::LightGreen,
            Cli::Color::LightCyan,
            Cli::Color::LightBlue,
            Cli::Color::Default
        );

        // Integer Types
        logger << "Integers[i|l|ll|u|ul|ull]: " << 0 << 0l << 0ll << 0u << 0ul << 0ull << "\n";

        // String based Types
        char Char[] = "Char *";
        const char *ConstChar = "ConstChar *";
        std::string String = "String";

        logger(LoggerType::Console, "{}{} {}{} {}{}\n",
            Cli::Color::LightRed, Char,
            Cli::Color::LightGreen, ConstChar,
            Cli::Color::LightBlue, String
        );

        // Wide-String based Types
        //wchar_t WChar_T[] = L"WChar_T *";
        //const wchar_t *ConstWChar_T = L"ConstWChar_T *";
        //std::wstring WString = L"WString";

        //logger("{}{} {}{} {}{}",
        //    Cli::Color::LightRed, WChar_T,
        //    Cli::Color::LightGreen, ConstWChar_T,
        //    Cli::Color::LightBlue, WString
        //);

        // Mixed String Types
        //logger << "Mixed String Types " << L"and Wide " << "... not wide " << L"and so on.\n";
    }

private:
    // Properties
    LogLevel mLogLevel;
    mutex mMutex {};
    vector<Scope<ILogger>> mSinks {};

    // States
    uint64_t mCounter {};
    bool mSkip = false;

    // Stream Buffer
    ostringstream mStreamBuffer { "" };
    LogLevel mStreamLogLevel { LogLevel::Default };
};

inline Logger &logger = Logger::Instance();

///
/// @brief  As good as a logger can be, we need something for applications where performance matters. Therefore these function templates are for convenience,
/// they will help removing unnecessary code in release and distribution builds, therefore they also override the log levels.
///

template<typename ...Args> void Log(const LogRecord &record, Args &&...args)            { logger(LogLevel::Default, record, args...); logger << "\n"; }
template<typename ...Args> void LogCaption(const LogRecord &record, Args &&...args)     { logger(LogLevel::Caption, record, args...); logger; }
template<typename ...Args> void LogDelimiter(const LogRecord &record, Args &&...args)   { logger(LogLevel::Delimiter, record, args...); logger; }
#ifdef APP_MODE_DEBUG
    template<typename T, typename ...Args> bool AppAssert(T *object, const LogRecord &record, Args &&...args) {
        if (!object) {
            logger(LogLevel::Fatal, record, args...); logger << "\n";
            return true;
        }
        return false;
    }
    template<typename T, typename ...Args> bool AppAssert(T object, const LogRecord &record, Args &&...args) {
        if (!object) {
            logger(LogLevel::Fatal, record, args...); logger << "\n";
            return true;
        }
        return false;
    }

    template<typename ...Args> void LogTrace(const LogRecord &record, Args &&...args)   { logger(LogLevel::Trace, record, args...); logger << "\n"; }
    template<typename ...Args> void LogDebug(const LogRecord &record, Args &&...args)   { logger(LogLevel::Debug, record, args...); logger << "\n"; }
    template<typename ...Args> void LogInfo(const LogRecord &record, Args &&...args)    { logger(LogLevel::Info,  record, args...); logger << "\n"; }
#elif APP_MODE_RELEASE
    template<typename ...Args> void AppAssert([[maybe_unused]] Args &&...args)          {}

    template<typename ...Args> void LogTrace([[maybe_unused]] Args &&...args)           {}
    template<typename ...Args> void LogDebug([[maybe_unused]] Args &&...args)           {}
    template<typename ...Args> void LogInfo(const LogRecord &record, Args &&...args)    { logger(LogLevel::Info,  record, args...); logger << "\n"; }
#elif APP_MODE_DISTRIBUTION
    template<typename ...Args> void AppAssert([[maybe_unused]] Args &&...args)          {}

    template<typename ...Args> void LogTrace([[maybe_unused]] Args &&...args)           {}
    template<typename ...Args> void LogDebug([[maybe_unused]] Args &&...args)           {}
    template<typename ...Args> void LogInfo([[maybe_unused]] Args &&...args)            {}
#endif
template<typename ...Args> void LogWarning(const LogRecord &record, Args &&...args)     { logger(LogLevel::Warn,   record, args...); logger << "\n"; }
template<typename ...Args> void LogError(const LogRecord &record, Args &&...args)       { logger(LogLevel::Error,  record, args...); logger << "\n"; }
template<typename ...Args> void LogFatal(const LogRecord &record, Args &&...args)       { logger(LogLevel::Fatal,  record, args...); logger << "\n"; }

}

// Global Overloads
namespace std {

template <>
struct formatter<Ultra::LogLevel> {
    constexpr auto parse(format_parse_context &context) {
        return context.begin();
    }

    auto format(const Ultra::LogLevel &level, format_context &context) const {
        switch (level) {
            case Ultra::LogLevel::Trace:    { return format_to(context.out(), " [ Trace ] "); }
            case Ultra::LogLevel::Debug:    { return format_to(context.out(), " [ Debug ] "); }
            case Ultra::LogLevel::Info:     { return format_to(context.out(), " [ Info  ] "); }
            case Ultra::LogLevel::Warn:     { return format_to(context.out(), " [ Warn  ] "); }
            case Ultra::LogLevel::Error:    { return format_to(context.out(), " [ Error ] "); }
            case Ultra::LogLevel::Fatal:    { return format_to(context.out(), " [ Fatal ] "); }

            case Ultra::LogLevel::Caption: {
                return format_to(context.out(),
                    "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n");
            }
            case Ultra::LogLevel::Delimiter: {
                return format_to(context.out(),
                    "----------------------------------------------------------------\n"
                );
            }

            default: { return format_to(context.out(), ""); }
        }
    }
};

}

// Preprocessor Macro Extensions
export import "Logger.h";
