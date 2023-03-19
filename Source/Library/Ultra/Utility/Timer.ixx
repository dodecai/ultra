export module Ultra.Utility.Timer;

// Library
import Ultra.Core;

export namespace Ultra {

///
/// @brief Timer Units:
/// Seconds (s), Milliseconds (ms), Microseconds (µs), Nanoseconds (ns)
///
enum class TimerUnit: uint8_t {
    Seconds,
    Milliseconds,
    Microseconds,
    Nanoseconds,
};

///
/// @brief Timer: Counts ticks until GetDeltaTime or GetDeltaTimeAs is called.
/// @tparam <T>:  Any supported floating-point type.
/// 
template <std::floating_point T = float>
class Timer {
    // Types
    using Clock = std::chrono::steady_clock;
    using Timespan = std::chrono::duration<T, std::milli>;
    using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;

public:
    // Default
    Timer(): mStartTime(Clock::now()) {}
    ~Timer() = default;

    // Accessors
    /// Retrive delta time in ms
    inline const T GetDeltaTime() {
        Timespan duration = CalculateDuration();
        Reset();
        return duration.count();
    }
    /// Retrive delta time in specified unit (s = default, ms, µs, ns)
    inline const T GetDeltaTimeAs(TimerUnit unit = TimerUnit::Seconds) {
        T duration {};
        switch (unit) {
            case TimerUnit::Seconds:		{ duration = std::chrono::duration<T>(CalculateDuration()).count();             break; }
            case TimerUnit::Milliseconds:	{ duration = std::chrono::duration<T, std::milli>(CalculateDuration()).count(); break; }
            case TimerUnit::Microseconds:	{ duration = std::chrono::duration<T, std::micro>(CalculateDuration()).count(); break; }
            case TimerUnit::Nanoseconds:	{ duration = std::chrono::duration<T, std::nano>(CalculateDuration()).count();  break; }
            default: { break; }
        }
        Reset();
        return std::move(duration);
    }
    /// Retrive a delta time slice in ms
    inline const T Now() {
        return (Timespan{ CalculateDuration() }).count();
    }

private:
    // Methods
    inline Timespan CalculateDuration() { return { Clock::now() - mStartTime }; }
    inline void Reset() { mStartTime = Clock::now(); }

private:
    // Properties
    Timestamp mStartTime;
};

///
/// @brief Timestamp: Holds the delta time in milliseconds for floating-point types.
///
class Timestamp {
public:
    // Default
    Timestamp(double time = 0.0): mTime { time } {}
    ~Timestamp() = default;

    // Accessors
    inline const double GetSeconds() { return mTime / 1000.0; }
    inline const double GetMilliseconds() { return mTime; }

    // Operators
    inline operator double() { return GetSeconds(); }

private:
    // Properties
    double mTime;
};

}
