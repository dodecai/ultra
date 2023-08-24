// Module
export module Ultra.Utility.DateTime;

// Library
import Ultra.Core;

export namespace Ultra {

///
/// @brief DateTime: Delivers current date/time/runtime/timestamp in ISO 8601 format.
/// Simply use 'apptime' under the Ultra Namespace to retrieve the desired information.
/// 
class DateTime: public SteadyObject {
private:
    // Types
    using SystemClock = std::chrono::system_clock;
    using Timespan = std::chrono::duration<double, std::micro>;
    using Timestamp = std::chrono::time_point<std::chrono::system_clock>;

    // Default
    DateTime(): mStartTime(SystemClock::now()) {}
    ~DateTime() = default;

public:
    // Returns an instance to the global static object
    static DateTime &Instance() {
        static DateTime instance;
        return instance;
    }

    // Accessors
    /// Retrieve current date in ISO 8601 format 'YYYY-mm-dd'
    inline string GetDate() const { return GetTicks("{:%Y-%m-%d}"); }
    /// Retrieve current time in ISO 8601 format 'HH:mm:ss.cccccc'
    inline string GetTime() const { return GetTicks("{:%H:%M:%S}"); }
    /// Retrieve runtime in ISO 8601 format 'PddTHH:mm:ss'
    inline string GetRuntime() const { return GetRuntimeTicks(); }
    /// Retrieve timestamp in ISO 8601 format 'YYYY-mm-ddTHH:mm:ss.cccccc'
    inline string GetTimeStamp() const { return GetTicks(); }

private:
    // Methods
    inline string GetTicks (const string_view &format = "{:%Y-%m-%dT%H:%M:%S}") const {
        auto args = std::make_format_args(SystemClock::now());
        try {
            return std::vformat(format, args);
        } catch (const std::exception &ex) {
            return ex.what();
        }
    }
    inline string GetRuntimeTicks (const string_view &format = "P{:02d}DT{:02d}:{:02d}:{:02d}.{:06d}") const {
        //// Option A: "P{:%H:%M:%S}" (lacks support for years, months and days)
        //auto elapsed = SystemClock::now() - mStartTime;
        //auto args = std::make_format_args(elapsed);
        //try {
        //    return std::vformat(format, args);
        //} catch (const std::exception &ex) {
        //    return ex.what();
        //}

        // Option B: "P{0:02d}DT{1:02d}:{2:02d}:{3:02d}"
        using namespace std::chrono;
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(SystemClock::now() - mStartTime);

        auto year = duration_cast<years>(elapsed);
        elapsed -= year;
        auto month = duration_cast<months>(elapsed);
        elapsed -= month;
        auto day = duration_cast<days>(elapsed);
        elapsed -= day;
        auto hour = duration_cast<hours>(elapsed);
        elapsed -= hour;
        auto minute = duration_cast<minutes>(elapsed);
        elapsed -= minute;
        auto second = duration_cast<seconds>(elapsed);
        elapsed -= second;
        auto millisecond = duration_cast<microseconds>(elapsed);

        auto args = std::make_format_args(day.count(), hour.count(), minute.count(), second.count(), millisecond.count());
        try {
            return std::vformat(format, args);
        } catch (const std::exception &ex) {
            return ex.what();
        }
    }

private:
    // Properties
    const Timestamp mStartTime;
};

inline DateTime &apptime = DateTime::Instance();

}
