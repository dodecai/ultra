export module Ultra.Core.Timer;

import <chrono>;

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
class Timer {
public:
    // Default
    Timer(): mStartTime(std::chrono::steady_clock::now()) {}
    ~Timer() = default;

    // Accessors
    /// Retrieve delta time in ms
    inline const float GetDeltaTime() {
        std::chrono::duration<float, std::milli> duration = CalculateDuration();
        Reset();
        return duration.count();
    }
    /// Retrieve delta time in specified unit (s = default, ms, µs, ns)
    inline const float GetDeltaTimeAs(TimerUnit unit = TimerUnit::Seconds) {
        float duration {};
        switch (unit) {
            case TimerUnit::Seconds:		{ duration = std::chrono::duration<float>(CalculateDuration()).count();             break; }
            case TimerUnit::Milliseconds:	{ duration = std::chrono::duration<float, std::milli>(CalculateDuration()).count(); break; }
            case TimerUnit::Microseconds:	{ duration = std::chrono::duration<float, std::micro>(CalculateDuration()).count(); break; }
            case TimerUnit::Nanoseconds:	{ duration = std::chrono::duration<float, std::nano>(CalculateDuration()).count();  break; }
            default: { break; }
        }
        Reset();
        return std::move(duration);
    }
    /// Retrieve a delta time slice in ms
    inline const float Now() {
        return (std::chrono::duration<float, std::milli>{ CalculateDuration() }).count();
    }

private:
    // Methods
    inline std::chrono::duration<float, std::milli> CalculateDuration() { return { std::chrono::steady_clock::now() - mStartTime }; }
    inline void Reset() { mStartTime = std::chrono::steady_clock::now(); }

private:
    // Properties
    std::chrono::time_point<std::chrono::steady_clock> mStartTime;
};

}
