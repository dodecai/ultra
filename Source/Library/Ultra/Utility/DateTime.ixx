// Module
export module Ultra.Utility.DateTime;

// Library
import Ultra.Core;

export namespace Ultra {

///
/// @brief DateTime: Delivers current date/time/runtime/timestamp in ISO 8601 format.
/// Simply use apptime under the Ultra Namespace to retrieve the desired information.
/// 
class DateTime {
    // Types
    using SystemClock = std::chrono::system_clock;
    using Timespan = std::chrono::duration<double, std::micro>;
    using Timestamp = std::chrono::time_point<std::chrono::system_clock>;

    // Default
    DateTime(): mStartTime(SystemClock::now()) {}
    DateTime(const DateTime &) = delete;
    DateTime(DateTime &&) noexcept = delete;
    ~DateTime() = default;

public:
    /// Returns an intance to the global static object
    static DateTime &Instance() {
        static DateTime instance;
        return instance;
    }

    // Accessors
    /// Retrive current date in ISO 8601 format 'YYYY-mm-dd'
    inline const string GetDate() { return GetTicks("{:%Y-%m-%d}"); }
    /// Retrive current time in ISO 8601 format 'HH:mm:ss.cccccc'
    inline const string GetTime() { return GetTicks("{:%H:%M:%S}"); }
    /// Retrive runtime in ISO 8601 format 'PddTHH:mm:ss'
    inline const string GetRuntime() { return GetRuntimeTicks(); }
    /// Retrive timestamp in ISO 8601 format 'YYYY-mm-ddTHH:mm:ss.cccccc'
    inline const string GetTimeStamp() { return GetTicks(); }

private:
    // Methods
    inline const std::string GetTicks(const std::string_view format = "{:%Y-%m-%dT%H:%M:%S}") {
        auto args = std::make_format_args(SystemClock::now());
        try {
            string result = std::vformat(format, args);
            return result;
        } catch (const std::exception &ex) {
            return {};
        }
    }
    inline const std::string GetRuntimeTicks(const std::string_view format = "P{0:02d}DT{1:02d}:{2:02d}:{3:02d}") {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(SystemClock::now() - mStartTime);
        
        //auto years = (elapsed / (86400 * 365));
        //auto months = (elapsed % (86400 * 365)) / (86400 * 30);
        auto days = ((elapsed % (86400 * 30)) / 86400);
        auto hours = (elapsed % 86400) / 3600;
        auto minutes = (elapsed % 3600) / 60;
        auto seconds = elapsed % 60;

        auto args = std::make_format_args(days.count(), hours.count(), minutes.count(), seconds.count());
        try {
            string result = std::vformat(format, args);
            return result;
        } catch (const std::exception &ex) {
            return {};
        }
    }

private:
    // Properties
    const Timestamp mStartTime;
};

inline DateTime &apptime = DateTime::Instance();

}
