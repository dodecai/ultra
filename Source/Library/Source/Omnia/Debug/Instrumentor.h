#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <mutex>
#include <iomanip>
#include <string>
#include <sstream>
#include <thread>

#define APP_PROFILE 0

namespace Omnia { namespace Profiler {

//using TimeStep = std::chrono::duration<double, std::nano>;
//
//struct Result {
//	std::string Name;
//	std::thread::id TrheadID;
//
//	TimeStep StartTime;
//	std::chrono::nanoseconds DeltaTime;
//};
//
//struct Session {
//	std::string Name;
//};
//
//
//class Instrumentor {
//	std::mutex m_Mutex;
//	InstrumentationSession* m_CurrentSession;
//	std::ofstream m_OutputStream;
//
//public:
//	Instrumentor()
//		: m_CurrentSession(nullptr) {
//	}
//
//	static Instrumentor& Get() {
//		static Instrumentor instance;
//		return instance;
//	}
//
//	void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
//		std::lock_guard lock(m_Mutex);
//		if (m_CurrentSession) {
//			// If there is already a current session, then close it before beginning new one.  Subsequent profiling output meant for the original session will end up in the
//			// newly opened session instead.  That's better than having badly formatted  profiling output.
//			//if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
//			//{
//			//	HZ_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
//			//}
//			InternalEndSession();
//		}
//		m_OutputStream.open(filepath);
//
//		if (m_OutputStream.is_open()) {
//			m_CurrentSession = new InstrumentationSession({name});
//			WriteHeader();
//		} else {
//			//if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
//			//{
//			//	HZ_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
//			//}
//		}
//	}
//
//	void EndSession() {
//		std::lock_guard lock(m_Mutex);
//		InternalEndSession();
//	}
//
//	void WriteProfile(const ProfileResult& result) {
//		std::stringstream json;
//
//		json << std::setprecision(3) << std::fixed;
//		json << ",{";
//		json << "\"cat\": \"function\",\n";
//		json << "\"dur\": " << (result.ElapsedTime.count()) << ',\n';
//		json << "\"name\": \"" << result.Name << "\",\n";
//		json << "\"ph\": \"X\",\n";
//		json << "\"pid\": 0,\n";
//		json << "\"tid\": " << result.ThreadID << ",\n";
//		json << "\"ts\": " << result.Start.count() << ",\n";
//		json << "}";
//
//		std::lock_guard lock(m_Mutex);
//		if (m_CurrentSession) {
//			m_OutputStream << json.str();
//			m_OutputStream.flush();
//		}
//	}
//
//private:
//	void WriteHeader() {
//		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
//		m_OutputStream.flush();
//	}
//
//	void WriteFooter() {
//		m_OutputStream << "]}";
//		m_OutputStream.flush();
//	}
//
//	// Note: you must already own lock on m_Mutex before  calling InternalEndSession()
//	void InternalEndSession() {
//		if (m_CurrentSession) {
//			WriteFooter();
//			m_OutputStream.close();
//			delete m_CurrentSession;
//			m_CurrentSession = nullptr;
//		}
//	}
//};
//
//class Timer {
//	const char* m_Name;
//	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
//	bool m_Stopped;
//
//public:
//	Timer(const char* name):
//		m_Name(name), m_Stopped(false) {
//		m_StartTimepoint = std::chrono::steady_clock::now();
//	}
//
//	~Timer() {
//		if (!m_Stopped) Stop();
//	}
//
//	void Stop() {
//		auto endTimepoint = std::chrono::steady_clock::now();
//		auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
//		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();
//
//		Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
//
//		m_Stopped = true;
//	}
//};
//
//
//namespace Utilities {
//
//template <size_t N>
//struct ChangeResult {
//	char Data[N];
//};
//
//template <size_t N, size_t K>
//constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K]) {
//	ChangeResult<N> result = {};
//
//	size_t srcIndex = 0;
//	size_t dstIndex = 0;
//	while (srcIndex < N) {
//		size_t matchIndex = 0;
//		while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
//			matchIndex++;
//		if (matchIndex == K - 1)
//			srcIndex += matchIndex;
//		result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
//		srcIndex++;
//	}
//	return result;
//}
//
//}

}}


namespace Omnia {

/// ToDo: Remove this part
using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult {
	std::string Name;

	FloatingPointMicroseconds Start;
	std::chrono::microseconds ElapsedTime;
	std::thread::id ThreadID;
};

struct InstrumentationSession {
	std::string Name;
};

class Instrumentor {
	std::mutex m_Mutex;
	InstrumentationSession* m_CurrentSession;
	std::ofstream m_OutputStream;

public:
	Instrumentor()
		: m_CurrentSession(nullptr) {
	}

	static Instrumentor& Get() {
		static Instrumentor instance;
		return instance;
	}

	void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
		std::lock_guard lock(m_Mutex);
		if (m_CurrentSession)
		{
			// If there is already a current session, then close it before beginning new one.
			// Subsequent profiling output meant for the original session will end up in the
			// newly opened session instead.  That's better than having badly formatted
			// profiling output.
			//if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
			//{
			//	HZ_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
			//}
			InternalEndSession();
		}
		m_OutputStream.open(filepath);

		if (m_OutputStream.is_open())
		{
			m_CurrentSession = new InstrumentationSession({name});
			WriteHeader();
		}
		else
		{
			//if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
			//{
			//	HZ_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
			//}
		}
	}

	void EndSession() {
		std::lock_guard lock(m_Mutex);
		InternalEndSession();
	}

	void WriteProfile(const ProfileResult& result) {
		std::lock_guard lock(m_Mutex);
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
		json << "\"name\":\"" << result.Name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.ThreadID << ",";
		json << "\"ts\":" << result.Start.count();
		json << "}";

		if (m_CurrentSession)
		{
			m_OutputStream << json.str();
			m_OutputStream.flush();
		}
	}

private:
	void WriteHeader() {
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
		m_OutputStream.flush();
	}

	void WriteFooter() {
		m_OutputStream << "]}";
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
};

class InstrumentationTimer {
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;

public:
	InstrumentationTimer(const char* name):
		m_Name(name), m_Stopped(false) {
		m_StartTimepoint = std::chrono::steady_clock::now();
	}

	~InstrumentationTimer() {
		if (!m_Stopped) Stop();
	}

	void Stop() {
		auto endTimepoint = std::chrono::steady_clock::now();
		auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

		Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

		m_Stopped = true;
	}
};

namespace InstrumentorUtils {

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

#if APP_PROFILE
	// Resolve which function signature macro will be used. Note that this only  is resolved when the (pre)compiler starts, so the syntax highlighting  could mark the wrong one in your editor!
	#if defined(_MSC_VER) 
	#define APP_FUNCTION_SIGNATURE  __func__
	#endif
	#define APP_PROFILE_BEGIN_SESSION(name, filepath)	::Omnia::Instrumentor::Get().BeginSession(name, filepath)
	#define APP_PROFILE_END_SESSION()					::Omnia::Instrumentor::Get().EndSession()
	#define APP_PROFILE_SCOPE(name)						constexpr auto fixedName = ::Omnia::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
															::Omnia::InstrumentationTimer timer##__LINE__(fixedName.Data)
	#define APP_PROFILE_FUNCTION()						APP_PROFILE_SCOPE(APP_FUNCTION_SIGNATURE)
#else
	#define APP_PROFILE_BEGIN_SESSION(name, filepath)
	#define APP_PROFILE_END_SESSION()
	#define APP_PROFILE_SCOPE(name)
	#define APP_PROFILE_FUNCTION()
#endif
