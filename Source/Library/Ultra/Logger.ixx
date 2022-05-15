module;

export module Ultra.Logger;

import <iostream>;
import <ostream>;

using std::cout;
using std::ostream;

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

class Logger {
    // Constructors and Deconstructor
    Logger() = default;

public:
    Logger(const Logger &) = delete;
    Logger(Logger &&) noexcept = delete;
    static Logger &Instance() {
        static Logger instance;
        return instance;
    }
    ~Logger() = default;

    // Methods
    template <typename_logmodifier T>
    Logger &operator<<(T &&data) {
        return (*this);
    }

    template<typename T>
    Logger &operator<<(T &&data) {
        mStream << std::forward<T>(data);
        return (*this);
    }
    
    Logger &operator<<(ostream &(*T)(ostream &)) {
        mStream << T;
        return (*this);
    }

private:
    ostream &mStream = cout;
};

Logger &logger = Logger::Instance();

///
/// @brief  As good as a logger can be, we need something for applications where performance matters. Therefore these function templates are for convenience,
/// they will help removing unaccessary code in release and distribution builds, therefore they also override the log levels.
///
template<typename ...Args> void AppLog(Args &&...args)			{ logger << LogLevel::Default   ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogTrace(Args &&...args)		{ logger << LogLevel::Trace     ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogDebug(Args &&...args)		{ logger << LogLevel::Debug     ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogInfo(Args &&...args)		{ logger << LogLevel::Info	    ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogWarning(Args &&...args)   { logger << LogLevel::Warn	    ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogError(Args &&...args)		{ logger << LogLevel::Error	    ; (logger << ... << args); logger << "\n"; }
template<typename ...Args> void AppLogFatal(Args &&...args)	    { logger << LogLevel::Fatal     ; (logger << ... << args); logger << "\n"; }

}
