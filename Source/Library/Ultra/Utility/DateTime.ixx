// Module
export module Ultra.Utility.DateTime;

// Default
import <chrono>;
import <format>;

// Usings
using namespace std::chrono;
using namespace std::chrono_literals;

export using namespace std::literals::chrono_literals;

export namespace Ultra { namespace Utility {

class DateTime {
public:
    DateTime() = default;
    template <typename Rep, typename Period>
    constexpr DateTime(const duration<Rep, Period> &duration):
        Nanoseconds(duration_cast<nanoseconds>(duration).count()) {
    }
    ~DateTime() = default;

    // Converters
    template <typename T = long long>
    auto AsNanoseconds() const { return static_cast<T>(Nanoseconds.count()); }
    template <typename T = long long>
    auto AsMicroseconds() const { return static_cast<T>(Nanoseconds.count()) / static_cast<T>(1000); }
    template <typename T = long long>
    auto AsMilliseconds() const { return static_cast<T>(Nanoseconds.count()) / static_cast<T>(1000000); }
    template <typename T = long long>
    auto AsSeconds() const { return static_cast<T>(Nanoseconds.count()) / static_cast<T>(1000000000); }

    // Accessors
    template <typename T = long long>
    static constexpr DateTime &GetNanoseconds(const T &nanoseconds) { return Time(duration<T, std::nano>(nanoseconds));	}
    template <typename T = long long>
    static constexpr DateTime &GetMicroseconds(const T &microseconds) { return Time(duration<T, std::micro>(microseconds)); }
    template <typename T = long long>
    static constexpr DateTime &GetMilliseconds(const T &milliseconds) { return Time(duration<T, std::micro>(milliseconds)); }
    template <typename T = long long>
    static constexpr DateTime &GetSeconds(const T &seconds) { return Time(duration<T>(seconds)); }

    static const std::string GetTimeStamp(const std::string &format = "%FT%T") {
        // Get timestamp
        //auto nows = floor<std::chrono::microseconds>(high_resolution_clock::now());
        auto now = floor<std::chrono::microseconds>(system_clock::now());

        std::stringstream ss;
        ss << "{:" << format << "}";

        // ToDo: ss.str() doesn't work anymore
        return std::format("{:%FT%T}", now);
    }
    static const std::string GetIsoDate() {
        return GetDateTime(DateFormat);
    }
    static const std::string GetIsoTime() {
        return GetDateTime(TimeFormat);
    }
    static const DateTime Now() {
        return duration_cast<nanoseconds>(high_resolution_clock::now() - StartTime);
    }

    // Comparision
    auto operator<=>(const DateTime &rhs) const { return Nanoseconds.count() <=> rhs.Nanoseconds.count(); }

    // Operators
    template <typename Rep, typename Period>
    operator duration<Rep, Period>() const { return duration_cast<duration<Rep, Period>>(Nanoseconds); }

    template <typename Period = std::ratio<1, 1>>
    float Mod(const DateTime &other) {
        return std::modf(duration_cast<duration<double, Period>>(Nanoseconds), duration_cast<duration<double, Period>>(other.Nanoseconds));
    }

private:
    static const std::string GetDateTime(const std::string &format) {
        auto now = floor<std::chrono::microseconds>(system_clock::now());

        std::stringstream ss;
        ss << "{:" << format << "}";

        // ToDo: ss.str() doesn't work anymore
        return std::format("{:%FT%T}", now);
    }

    static const inline std::string DateFormat = "%Y-%m-%d";
    static const inline std::string TimeFormat = "%H:%M:%S";

    static const time_point<high_resolution_clock> StartTime;
    nanoseconds Nanoseconds = {};
};

inline const time_point<high_resolution_clock> DateTime::StartTime(high_resolution_clock::now());

}

inline Utility::DateTime apptime;

}
