// Module
export module Ultra.Logger;

// Default
import <iostream>;
import <source_location>;

// Library
import Ultra.Core;
import Ultra.System.Cli;
import Ultra.Utility.DateTime;
import Ultra.Utility.String;

// Usings
using std::cout;
using std::source_location;
using std::wcout;
using std::wostream;

export namespace Ultra {

// Enumerations
enum class LogLevel {
    // Log Levels
    Trace		= 0x1,	// Message (Level 1): Everything that could be interesting in the future.
    Debug		= 0x2,	// Message (Level 2): Debugging messages, which can help at parts, where we are unsure if the code will ever fail.
    Info		= 0x3,	// Message (Level 3): Information that is maybe usefull for gattering basic system data.
    Warn		= 0x4,	// Message (Level 4): Has no impact on the application itself but should get sometime fixed.
    Error		= 0x5,  // Message (Level 5): Error messages that have no impact on runtime execution.
    Fatal   	= 0x6,	// Message (Level 6): Critical messages which will break runtime execution.
    Default		= 0x7,	// Message (Level 7): Default output with nothing added

    // Types
    Caption		= 0x8,	// Structure the log by a given caption
    Delimiter	= 0x9,	// Structure the log with a delimiter
};

// Concepts
template <typename T>
concept typename_logmodifier = std::is_same_v<T, LogLevel>;

// Overrides
template <typename_logmodifier T>
inline ostream &operator<<(ostream &os, T type) {
    return os << static_cast<size_t>(type);
}

///
/// @brief This logger fullfills all your basic needs, and accepts nearly everything you throw at it.
/// The motivation behind this was an easy to use, but performant logger, you pay for what you use. It's
/// basically an extension to cout with some gimmics.
/// @type Singleton
///
class Logger {
    // Constructors and Deconstructor
    Logger() { SetLevel(mLogLevel); };
    Logger(const Logger &) = delete;
    Logger(Logger &&) noexcept = delete;
    ~Logger() = default;

public:
    // Get the global instance to the logger (... and yeah thread-safe since C++11 accodring to researches!)
    static Logger &Instance() {
        static Logger instance;
        return instance;
    }

    // Operators
    Logger &operator=(const Logger &) { return Instance(); }
    Logger &operator=(Logger &&) noexcept { return Instance(); }

    // Accessors
    const auto &GetCounter() const { return mCounter; }
    const auto &GetLevel() const { return mLogLevel; }

    // Mutators
    void SetLevel(const LogLevel level) {
        std::unique_lock<mutex> lock(mSync);
        if (level > LogLevel::Fatal) {
            mLogLevel = LogLevel::Fatal;
        } else {
            mLogLevel = level;
        }
    }

    // Methods
    template <typename_logmodifier T>
    Logger &operator<<(T &&data) {
        std::unique_lock<mutex> lock(mSync);

        if constexpr (std::is_same_v<T, LogLevel>) {
            if (data >= mLogLevel) {
                mSkip = false;
                auto timestamp = apptime.GetTimeStamp();
                switch (data) {
                    case LogLevel::Fatal: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::Red << "[ Fatal ] ";
                        break;
                    }
                    case LogLevel::Error: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightRed << "[ Error ] ";
                        break;
                    }
                    case LogLevel::Warn: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightYellow << "[ Warn  ] ";
                        break;
                    }
                    case LogLevel::Info: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightGray << "[ Info  ] ";
                        break;
                    }
                    case LogLevel::Debug: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightGreen << "[ Debug ] ";
                        break;
                    }
                    case LogLevel::Trace: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightMagenta << "[ Trace ] " ;
                        break;
                    }

                    case LogLevel::Caption: {
                        mCaptionActive = true;
                        mStream << Cli::Color::LightBlue << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
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
    Logger &operator<<(T &&data) {
        std::unique_lock<mutex> lock(mSync);
        if (mSkip) return (*this);
        mStream << std::forward<T>(data);
        return (*this);
    }

    template<typename T>
    Logger &operator<<(T &&data) {
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
    
    Logger &operator<<(ostream &(*T)(ostream &)) {
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

    // Show whe what you got...
    static void Test() {
        // LogLevels
        Logger::Instance() << LogLevel::Caption << "Caption" << "\n";
        Logger::Instance() << LogLevel::Default << "Default" << "\n";
        Logger::Instance() << LogLevel::Delimiter;
        Logger::Instance() << LogLevel::Fatal   << "Fatal" << "\n";
        Logger::Instance() << LogLevel::Error   << "Error" << "\n";
        Logger::Instance() << LogLevel::Warn    << "Warn" << "\n";
        Logger::Instance() << LogLevel::Info    << "Info" << "\n";
        Logger::Instance() << LogLevel::Debug   << "Debug" << "\n";
        Logger::Instance() << LogLevel::Trace   << "Trace" << "\n";
        Logger::Instance() << LogLevel::Delimiter;

        // Multi-Color
        Logger::Instance()  << LogLevel::Default << "This "
                            << Cli::Background::White << Cli::Color::Black << "is "
                            << Cli::Background::Default << Cli::Color::LightBlue << "a "
                            << Cli::Color::LightRed << "multi" << Cli::Color::LightGray << "-"
                            << Cli::Color::LightGreen << "color" << Cli::Color::LightCyan << "-"
                            << Cli::Color::LightBlue << "test"
                            << Cli::Color::Default << "!\n";

        // Integer Types
        Logger::Instance() << "Integers[i|l|ll|u|ul|ull]: " << 0 << 0l << 0ll << 0u << 0ul << 0ull << "\n";

        // String based Types
        char Char[] = "Char *";
        const char *ConstChar = "ConstChar *";
        std::string String = "String";

        Logger::Instance()  << Cli::Color::LightRed     << Char         << " "
                            << Cli::Color::LightGreen   << ConstChar    << " "
                            << Cli::Color::LightBlue    << String       << "\n";

        // Wide-String based Types
        wchar_t WChar_T[] = L"WChar_T *";
        const wchar_t *ConstWChar_T = L"ConstWChar_T *";
        std::wstring WString = L"WString";
        
        Logger::Instance()  << Cli::Color::LightRed     << WChar_T      << L" "
                            << Cli::Color::LightGreen   << ConstWChar_T << L" "
                            << Cli::Color::LightBlue    << WString      << L"\n";
        Logger::Instance() << std::endl;

        // Mixed String Types
        Logger::Instance() << "Mixed String Types " << L"and Wide " << "... not wide " << L"and so on.\n";
    }

private:
    // Properties
    LogLevel mLogLevel = LogLevel::Trace;
    string mLocation;
    ostream &mStream = cout;
    wostream &mWStream = wcout;
    mutex mSync;

    // States
    size_t mCounter = 0;
    bool mCaptionActive = false;
    bool mSkip = false;
};

inline Logger &logger = Logger::Instance();

///
/// @brief  As good as a logger can be, we need something for applications where performance matters. Therefore these function templates are for convenience,
/// they will help removing unaccessary code in release and distribution builds, therefore they also override the log levels.
///
template<typename ...Args> void AppLog(Args &&...args)			{ logger << LogLevel::Default; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogTrace(Args &&...args)		{ logger << LogLevel::Trace  ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogDebug(Args &&...args)		{ logger << LogLevel::Debug  ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogInfo(Args &&...args)		{ logger << LogLevel::Info	 ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogWarning(Args &&...args)   { logger << LogLevel::Warn	 ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogError(Args &&...args)		{ logger << LogLevel::Error	 ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogFatal(Args &&...args)	    { logger << LogLevel::Fatal  ; (logger << ... << args); logger << "\n"; }

}
