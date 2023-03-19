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
    // Constructors and Deconstructor
    DateTime() = default;
    DateTime(const DateTime &) = delete;
    DateTime(DateTime &&) noexcept = delete;
    ~DateTime() = default;

public:
    static DateTime &Instance() {
        static DateTime instance;
        return instance;
    }

    template <typename Rep, typename Period>
    constexpr DateTime(const duration<Rep, Period> &duration):
        Nanoseconds(duration_cast<nanoseconds>(duration).count()) {
    }

    // Converters
    auto AsNanoseconds() const { return duration_cast<nanoseconds>(Nanoseconds); }
    auto AsMicroseconds() const { return duration_cast<microseconds>(Nanoseconds); }
    auto AsMilliseconds() const { return duration_cast<milliseconds>(Nanoseconds); }
    auto AsSeconds() const { return duration_cast<seconds>(Nanoseconds); }

    // Accessors
    template <typename T = long long>
    static constexpr DateTime GetNanoseconds(const T &nanoseconds) { return DateTime(duration<T, std::nano>(nanoseconds));	}
    template <typename T = long long>
    static constexpr DateTime GetMicroseconds(const T &microseconds) { return DateTime(duration<T, std::micro>(microseconds)); }
    template <typename T = long long>
    static constexpr DateTime GetMilliseconds(const T &milliseconds) { return DateTime(duration<T, std::micro>(milliseconds)); }
    template <typename T = long long>
    static constexpr DateTime GetSeconds(const T &seconds) { return DateTime(duration<T>(seconds)); }

    static const std::string GetDate() {
        return GetDateTime(DateFormat);
    }
    static const std::string GetTime() {
        return GetDateTime(TimeFormat);
    }
    static const std::string GetTimeStamp(const std::string &format = "{%FT%T}") {
        // Get timestamp
        //auto nows = floor<std::chrono::microseconds>(high_resolution_clock::now());
        auto now = floor<std::chrono::microseconds>(system_clock::now());

        std::stringstream ss;
        ss << "{:" << format << "}";

        return std::format("{:%FT%T}", now);
        return std::vformat(format, std::make_format_args(now));
        // ToDo: Dynamic Format
        // std::vformat(format, std::make_format_args(now));
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
    static const std::string GetDateTime(const std::string_view format = "{:%FT%T}") {
        auto now = floor<std::chrono::microseconds>(system_clock::now());
        return std::vformat(format, std::make_format_args(now));
    }

private:
    // Properties
    static const inline std::string DateFormat = "{:%F}"; // Default Date Format
    static const inline std::string TimeFormat = "{:%T}"; // Default Time Format

    static const time_point<steady_clock> StartTime; // StartTime: Automatically set during application startup
    nanoseconds Nanoseconds = {};
};

inline const time_point<steady_clock> DateTime::StartTime(steady_clock::now());

}

inline Utility::DateTime &apptime = Utility::DateTime::Instance();

}
