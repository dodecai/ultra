#pragma once

#include "Omnia/Core.h"

namespace Omnia {

/// @brief Timestamp which holds milliseconds in a double.
class Timestamp {
public:
	// Default
	Timestamp(double time = 0.0): Time{ time } {}
	~Timestamp() = default;

    // Accessors
    operator double() { return GetSeconds(); }
	const double GetSeconds() { return Time / 1000.0; }
	const double GetMilliseconds() { return Time; }

private:
    // Properties
    double Time;
};

/// @brief Timer that counts ticks until GetDeltaTime or Reset is called.
class Timer {
    // Definitions
	using Clock = std::chrono::high_resolution_clock;
	using Timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using Timespan = std::chrono::duration<double>;

public:
    // Enumerations
    enum class Unit: uint8_t {
        Seconds,
        Milliseconds,
        Microseconds,
        Nanoseconds,
    };

public:
	// Default
	Timer() { Reset(); }
	~Timer() = default;
    /// Reset timer manually (automatically done by GetDeltaTimeX)
    void Reset() { StartTime = Clock::now(); }

    // Accessors
	/// Retrive current clock tick
	const double Now() { return std::chrono::duration_cast<std::chrono::microseconds>(CalculateDuration()).count() / 1000.0; }
    /// Retrive delta time in milliseconds (default)
	const double GetDeltaTime() {
		double duration = std::chrono::duration_cast<std::chrono::microseconds>(CalculateDuration()).count() / 1000.0;
		Reset();
		return duration;
	}
    /// Retrive delta time in specified period (s, ms, µs, ns) */
	const double GetDeltaTimeAs(Unit unit = Unit::Seconds) {
		double duration = 0.0;
		switch (unit) {
			case Unit::Seconds:			{ duration = std::chrono::duration_cast<std::chrono::microseconds>(CalculateDuration()).count() / 1000000.0;    break; }
			case Unit::Milliseconds:	{ duration = std::chrono::duration_cast<std::chrono::microseconds>(CalculateDuration()).count() / 1000.0;	    break; }
			case Unit::Microseconds:	{ duration = std::chrono::duration_cast<std::chrono::microseconds>(CalculateDuration()).count();			    break; }
			case Unit::Nanoseconds:		{ duration = std::chrono::duration_cast<std::chrono::nanoseconds>(CalculateDuration()).count();				    break; }
			default:					{ break; }
		}
		Reset();
		return duration;
	}

private:
    // Internal
	Timespan CalculateDuration() {
		return { Clock::now() - StartTime };
	}

private:
    // Properties
    Timestamp StartTime;
};

}
