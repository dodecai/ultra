#pragma once

#include "Omnia/Omnia.pch"
#include "Omnia/Core.h"
#include "Omnia/System/Cli.h"
#include "Omnia/Utility/DateTime.h"

#pragma warning(push)
#pragma warning(disable: 26812)

namespace Omnia {

using std::is_same_v;
using std::mutex;
using std::ofstream;
using std::ostream;

enum class LogLevel: uint8_t {
	Trace		= 0x0,
	Debug		= 0x1,
	Info		= 0x2,
	Default		= 0x3,
	Warn		= 0x4,
	Error		= 0x5,
	Critical	= 0x6,
};

/**
 * @brief This logging class fullfills basic needs, it takes everything you throw at it (like cout), and offers you some nice colors and similar features like that.
*/
class Log {
	// Properties
	static inline string LogFile = "./Log/Ultra.log"s;
	static inline size_t Counter = 0;
	static inline size_t Processes = 0;
	static inline size_t Successes = 0;
	static inline size_t Warnings = 0;
	static inline size_t Failures = 0;

	ostream &os = std::cout;
	ostream *os2 = nullptr;
	static inline mutex Sync;
	static inline mutex SyncFile;
	static inline ofstream FileStream;

	// States
	static inline bool CaptionActive = false;

	// Constructors and Operators
	Log() = default;
	Log(const Log &) {}
	Log(Log &&) noexcept {}
	Log &operator=(const Log &) { return Instance(); }
	Log &operator=(const Log &&) noexcept { return Instance(); }

public:
	// Deconstructors
	~Log() { Close(); }

	// Enumerations
	enum DefaultTypes {
		// Basic Types
		Default		= 0x00,	// Message (Level 3): Default output with nothing added
		Caption		= 0x01,	// Structure the log by a given caption
		Delimiter	= 0x02,	// Structure the log with a delimiter

		// Message Types
		Critical	= 0x10,	// Message (Level 5): Critical messages which will break runtime execution.
		Error		= 0x11, // Message (Level 4): Error messages that have no impact on runtime execution.
		Warn		= 0x12,	// Message (Level 3): Has no impact on the application itself but should get sometime fixed.
		Info		= 0x13,	// Message (Level 2): Information that is maybe usefull for gattering basic system data.
		Debug		= 0x14,	// Message (Level 1): Debugging messages, which can help at parts, where we are unsure if the code will ever fail.
		Trace		= 0x15,	// Message (Level 0): Everything that could be interesting in the future.
	};

	enum ExtendedTypes {
		// Extended Types for Workflows with builtin counting system, and automatic results at the end of the log-file.
		Process		= 0x20,	// The process itself, which can be a success, warning or failure.
		Success		= 0x21,
		Warning		= 0x22,
		Failure		= 0x23,
	};

	// Methods
	static Log &Instance() {
		static Log instance;
		return instance;
	}

	void SetOutput(ostream &output) {
		os2 = &output;
	}

	template <typename T>
	Log &operator<<(const T &data) {
		std::ios::sync_with_stdio(false);
		std::unique_lock<mutex> lock(Sync);
		std::unique_ptr<stringstream> cli { new stringstream };
		std::unique_ptr<stringstream> file { new stringstream };

		if (!FileStream.is_open()) { Open(); }

		// Concat timestamp
		if constexpr (std::is_same_v<T, DefaultTypes>) {
			if (!(data == Caption || data == Default || data == Delimiter)) {
				os << Cli::Color::DarkGrey << apptime.GetTimeStamp("%Y-%m-%dT%H:%M") << " | "; 
				*file << apptime.GetTimeStamp() << " | ";
			}
		}

		// Basic and Messages: Set color and symbol
		if constexpr (std::is_same_v<T, DefaultTypes>) {
			switch (data) {
				case Caption:   {
					CaptionActive = true;
					os << Cli::Color::LightBlue << "\n";
					os << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
					*file << "\n";
					*file << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
					break;
				}
				case Delimiter: {
					os << Cli::Color::Yellow;
					os << "----------------------------------------------------------------\n";
					*file << "----------------------------------------------------------------\n";
					break;
				}

				case Critical:	{ os << Cli::Color::Red				<< "♦ ";    *file << "\n";  break; }
				case Error:     { os << Cli::Color::LightRed		<< "˃ ";    *file << "˃ ";  break; }
				case Warn:      { os << Cli::Color::LightYellow		<< "≡ ";    *file << "≡ ";  break; }
				case Info:      { os << Cli::Color::LightGrey		<< "✶ ";    *file << "✶ ";  break; }
				case Debug:     { os << Cli::Color::LightGreen		<< "■ ";	*file << "■ ";  break; }
				case Trace:		{ os << Cli::Color::LightMagenta	<< "◙ ";	*file << "◙ ";  break; }

				default:        os << Cli::Color::White << "";          *file << "";    break;
			}
			Write(file->str());
			Counter++;
			return (*this);
		}

		// Extended: Set color and symbol
		if constexpr (std::is_same_v<T, ExtendedTypes>) {
			switch (data) {
				case Process:   { os << Cli::Color::Cyan		<< "҉ ";    *file << "҉ ";		break; }
				case Success:   { os << Cli::Color::LightCyan	<< "  ○ ";  *file << "  ○ ";	Successes++; break; }
				case Warning:   { os << Cli::Color::LightYellow	<< "  □ ";	*file << "  □ ";	Warnings++; break; }
				case Failure:   { os << Cli::Color::LightRed	<< "  ♦ ";  *file << "  ♦ ";	Failures++; break; }
			}
			Write(file->str());
			Counter++;
			return (*this);
		}

		// Automatically reset to default color after newline
		if constexpr (std::is_same_v<T, char[2]>) {
			if (data == "\n"s || data == "\r"s || data == "\r\n"s) {
				if (CaptionActive) {
					os << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
					*file << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
					CaptionActive = false;
				}
				*cli << Cli::Style::Reset;
				*cli << Cli::Color::White << "";
			}
		}

		// Stream data
		*cli << data;
		*file << data;
		os << cli->str();
		Write(file->str());
		Counter++;
		return (*this);
	}
	Log &operator<<(ostream &(*T)(ostream &)) {
		std::ios::sync_with_stdio(false);
		if (CaptionActive) {
			os << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
			CaptionActive = false;
		}
		os << Cli::Style::Reset << Cli::Color::White << T;
		return (*this);
	}

	// Show all features
	static void Test() {
		Log::Instance() << Log::Caption << "Caption" << "\n";
		Log::Instance() << Log::Default << "Default" << "\n";
		Log::Instance() << Log::Delimiter;
		Log::Instance() << Log::Critical << "Critical" << "\n";
		Log::Instance() << Log::Error << "Error" << "\n";
		Log::Instance() << Log::Warn << "Warn" << "\n";
		Log::Instance() << Log::Info << "Info" << "\n";
		Log::Instance() << Log::Debug << "Debug" << "\n";
		Log::Instance() << Log::Trace << "Trace" << "\n";
		Log::Instance() << Log::Delimiter;
		Log::Instance() << Log::Process << "Process" << "\n";
		Log::Instance() << Log::Success << "Success" << "\n";
		Log::Instance() << Log::Warning << "Warning" << "\n";
		Log::Instance() << Log::Failure << "Failure" << "\n";
	}
private:
	// Log-File Handling
	static void Close() {
		if (Successes || Warnings || Failures) Processes += Successes + Warnings + Failures;
		WriteFooter();
		FileStream.close();
	}
	static void Open(const std::filesystem::path &object = LogFile) {
		auto Directory = object.parent_path();
		if (!Directory.empty()) std::filesystem::create_directories(Directory);
		FileStream.open(object);
		WriteHeader();
	}
	static void Write(const string &message) {
		std::unique_lock<mutex> lock(SyncFile);
		if (FileStream.is_open()) FileStream << message;
	}

	// Log-File Helpers
	static void WriteHeader() {
		Write("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
		Write("Ultra\n");
		Write("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	}
	static void WriteFooter() {
		Write("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	}
};

/** This is the preferred instance to the logging class. */
inline Log &applog = Log::Instance();

/** This is the legacy instance to the logging class. */
inline Log &appout = Log::Instance();

/*
* @brief As good as the logging class can be, we need something for applications where performance matters. Therefore the following function templates
*		 are for convenience, they will help removing unaccessary code in distribution builds.
*/
template<typename ...Args> void AppLog(Args &&...args)			{ applog << Log::Default	; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogInfo(Args &&...args)		{ applog << Log::Info		; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogWarning(Args &&...args)	{ applog << Log::Warn		; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogError(Args &&...args)		{ applog << Log::Error		; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogCritical(Args &&...args)	{ applog << Log::Critical	; (applog << ... << args); applog << "\n"; throw std::runtime_error(""); }

#define APP_LOG(...)			AppLog			("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_INFO(...)		AppLogInfo		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_WARN(...)		AppLogWarning	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_ERROR(...)		AppLogError		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_CRITICAL(...)	AppLogCritical	("[", __FUNCTION__, "]: ", __VA_ARGS__)

#ifdef APP_DEBUG_MODE
	template<typename T, typename ...Args> bool AppAssert(T *object, Args &&...args) {
		if (!object) {
			applog << Log::Critical; (applog << ... << args); applog << "\n";
            return true;
		}
        return false;
	}
	template<typename T, typename ...Args> bool AppAssert(T object, Args &&...args) {
		if (!object) {
			applog << Log::Critical; (applog << ... << args); applog << "\n";
            return true;
		}
        return false;
	}
	#define AppAssert(x, ...) if(AppAssert(x, __VA_ARGS__)) APP_DEBUGBREAK() // Workaround, at the debug break after the message.
	template<typename ...Args> void AppLogDebug(Args &&...args)		{ applog << Log::Debug		; (applog << ... << args); applog << "\n"; }
	template<typename ...Args> void AppLogTrace(Args &&...args)		{ applog << Log::Trace		; (applog << ... << args); applog << "\n"; }
	
	// Old-School: If anybody wishes preprocessor macros, we have no problem with it.
	#define APP_ASSERT(x, ...)		{ if(!(x)) { AppLogCritical("[", __FUNCTION__, "]: ", __VA_ARGS__); APP_DEBUGBREAK(); } }
	#define APP_LOG_DEBUG(...)		AppLogDebug		("[", __FUNCTION__, "]: ", __VA_ARGS__)
	#define APP_LOG_TRACE(...)		AppLogTrace		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#else
	// ToDo: Should we use empty functions or replace everything with nothing with the preprocesor?
	//template<typename T, typename ...Args> void AppAssert(T *object, Args &&...args) {}
	//template<typename ...Args> void AppLog(Args &&...args)			{}
	//template<typename ...Args> void AppLogInfo(Args &&...args)		{}
	//template<typename ...Args> void AppLogWarning(Args &&...args)		{}
	//template<typename ...Args> void AppLogError(Args &&...args)		{}
	//template<typename ...Args> void AppLogCritical(Args &&...args)	{}
	//template<typename ...Args> void AppLogDebug(Args &&...args)		{}
	//template<typename ...Args> void AppLogTrace(Args &&...args)		{}
	// ToDo: Should the application get killed if something goes "very" wrong, or stay up?
	//template<typename T, typename ...Args> void AppAssert(T *object, Args &&...args) {
	//	if (!object) {
	//		applog << Log::Critical; (applog << ... << args); applog << "\n";
	//		std::abort();
	//	}
	//}
	// Either works just fine...
	#define AppAssert(...);
	//#define AppLog(...);
	//#define AppLogInfo(...);
	//#define AppLogWarning(...);
	//#define AppLogError(...);
	//#define AppLogCritical(...);
	#define AppLogDebug(...);
	#define AppLogTrace(...);

	// Old-School
	#define APP_ASSERT(x, ...);
	//#define APP_LOG(...);
	//#define APP_LOG_INFO(...);
	//#define APP_LOG_WARN(...);	
	//#define APP_LOG_ERROR(...);	
	//#define APP_LOG_CRITICAL(...);
	#define APP_LOG_DEBUG(...);
	#define APP_LOG_TRACE(...);
#endif

}

#pragma warning(pop)
