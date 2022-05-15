#pragma once

#include "Omnia/Log.h"
#include "Omnia/Types.h"

// String Extensions
namespace Omnia {

template<typename T>
concept typename_string = std::is_same_v<string, T> || std::is_same_v<std::wstring, T>;

template<typename T>
concept hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

constexpr inline size_t operator"" _hash(const char *value, size_t count) {
    return value == "" ? 0 : std::hash<string>{}(value);
}

/// @brief This class offers easy to use string extensions
class String {
    // Default
    String() = delete;
    String(const String &) = delete;
    String(String &&) = delete;
    ~String() = delete;

public:
    static bool Contains(string_view value, string_view token, bool caseSensitive = false) noexcept {
        if (value.length() < token.length()) return false;
        if (caseSensitive) {
            return value.find(token) != string::npos;
        } else {
            auto it = std::search(value.begin(), value.end(), token.begin(), token.end(), [](const char char1, const char char2) {
                return std::tolower(char1) == std::tolower(char2);
            });
            return (it != value.end());
        }
    }
    static bool EndsWith(string_view value, string_view token, bool caseSensitive = false) noexcept {
        if (value.length() < token.length()) return false;
        if (caseSensitive) {
            return value.compare(value.length() - token.length(), token.length(), token) == 0;
        } else {
            auto it = std::search(value.begin(), value.end(), token.begin(), token.end(), [](const char char1, const char char2) {
                return std::tolower(char1) == std::tolower(char2);
            });
            return (it == value.end() - token.length());
        }
    }
    static bool StartsWith(string_view value, string_view token, bool caseSensitive = false) noexcept {
        if (value.length() < token.length()) return false;
        if (caseSensitive) {
            return value.compare(0, token.length(), token) == 0;
        } else {
            auto it = std::search(value.begin(), value.end(), token.begin(), token.end(), [](const char char1, const char char2) {
                return std::tolower(char1) == std::tolower(char2);
            });
            return (it == value.begin());
        }
    }

    static bool IsNumeric(string_view value) noexcept {
        return std::all_of(value.cbegin(), value.cend(), [](auto c) {
            return (c >= '0' && c <= '9') || c == '-' || c == ',' || c == '.';
        });
    }
    static bool IsDecimal(string_view value) noexcept {
        return std::all_of(value.cbegin(), value.cend(), [](auto c) {
            return (c >= '0' && c <= '9');
        });
    }
    static bool IsHexadecimal(string_view value) noexcept {
        return std::all_of(value.cbegin(), value.cend(), [](auto c) {
            return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c == 'x' || c == 'X');
        });
    }
    static bool IsOctal(string_view value) noexcept {
        return std::all_of(value.cbegin(), value.cend(), [](auto c) {
            return (c >= '0' && c <= '7');
        });
    }

    static string &Replace(string &value, string_view token, string_view to, bool caseSensitive = false) noexcept {
        if (value.empty() || token.empty() || value.length() < token.length()) return value;
        if (caseSensitive) {
            auto position = value.find(token);
            while (position != std::string::npos) {
                value.replace(position, token.length(), to);
                position = value.find(token, position + token.size());
            }
        } else {
            auto positionStart = std::search(value.begin(), value.end(), token.begin(), token.end(), [](const char char1, const char char2) {
                return std::tolower(char1) == std::tolower(char2);
            });
            while (positionStart != value.end()) {
                auto positionEnd = positionStart;
                std::advance(positionEnd, token.length());
                value.replace(positionStart, positionEnd, to);
                positionStart = std::search(positionStart, value.end(), token.begin(), token.end(), [](const char char1, const char char2) {
                    return std::tolower(char1) == std::tolower(char2);
                });
            }
        }
        return value;
    }
    static vector<string> Split(const string &value, char seperator) noexcept {
        stringstream stream(value);
        string token;
        vector<string> tokens;

        while (std::getline(stream, token, seperator)) {
            tokens.emplace_back(token);
        }
        return tokens;
    }
    template<typename_string T = string>
    static T &ToLower(T &value) noexcept {
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        return value;
    }
    template<typename_string T = string>
    static T &ToUpper(T &value) noexcept {
        std::transform(value.begin(), value.end(), value.begin(), ::toupper);
        return value;
    }

    static void Test() {
        string string00 = "First Second and SeConD Third";
        string string01 = "fiRst";
        string string02 = "second";
        string string03 = "ThIrD";
        string string04 = "-Foruth";

        string string10 = "-123,456.789";
        string string11 = "-123,456.789a";
        string string12 = "0123456789";
        string string13 = "0123456789a";
        string string14 = "0x123456789ABCDEF";
        string string15 = "0x123456789ABCDEFG";
        string string16 = "01234567";
        string string17 = "012345678";

        string string20 = "This is a very long sentance for a small string!";
        string string21 = "What the ... should i do. Maybe it would be my 1st try to 3489032840921384092384902318490823149203945803294.";
        std::wstring wstring20 = L"Dies ist ein sehr langer Satz für einen String, deshalb sollter er mit bedacht eingesetzt werden!";
        std::wstring wstring21 = L"Ein paar Zeichen zum testen #+äü+ö€";

        // Perform simple tests
        AppAssert(!String::StartsWith(string00, string01, true), "[Application::Utility::String]: '", string00, "' starts with '", string01, "' failed!");
        AppAssert(String::StartsWith(string00, string01), "[Application::Utility::String]: '", string00, "' starts with '", string01, "' failed!");
        AppAssert(!String::StartsWith(string00, string03), "[Application::Utility::String]: '", string00, "' starts with '", string03, "' failed!");

        AppAssert(!String::Contains(string00, string02, true), "[Application::Utility::String]: '", string00, "' contains '", string02, "' failed!");
        AppAssert(String::Contains(string00, string02), "[Application::Utility::String]: '", string00, "' contains '", string02, "' failed!");
        AppAssert(!String::Contains(string00, string04), "[Application::Utility::String]: '", string00, "' contains '", string04, "' failed!");
        
        AppAssert(!String::EndsWith(string00, string03, true), "[Application::Utility::String]: '", string00, "' ends with '", string03, "' failed!");
        AppAssert(String::EndsWith(string00, string03), "[Application::Utility::String]: '", string00, "' ends with '", string03, "' failed!");
        AppAssert(!String::EndsWith(string00, string01), "[Application::Utility::String]: '", string00, "' ends with '", string01, "' failed!");

        AppAssert(String::IsNumeric(string10), "[Application::Utility::String]: '", string10, "' is numeric failed!");
        AppAssert(!String::IsNumeric(string11), "[Application::Utility::String]: '", string11, "' is numeric failed!");

        AppAssert(String::IsDecimal(string12), "[Application::Utility::String]: '", string12, "' is decimal failed!");
        AppAssert(!String::IsDecimal(string13), "[Application::Utility::String]: '", string13, "' is decimal failed!");

        AppAssert(String::IsHexadecimal(string14), "[Application::Utility::String]: '", string14, "' is hexadecimal failed!");
        AppAssert(!String::IsHexadecimal(string15), "[Application::Utility::String]: '", string15, "' is hexadecimal failed!");

        AppAssert(String::IsOctal(string16), "[Application::Utility::String]: '", string16, "' is octal failed!");
        AppAssert(!String::IsOctal(string17), "[Application::Utility::String]: '", string17, "' is octal failed!");
        
        // Perform advanced test
        String::Replace(string00, "second", "sec", true);
        String::Replace(string00, "second", "sec");
        String::Replace(string11, "a", "b");

        auto vector1 = String::Split(string00, ' ');
        auto vector2 = String::Split(string10, ',');

        String::ToLower(string20);
        String::ToUpper(string20);
        String::ToLower(string21);
        String::ToUpper(string21);

        String::ToLower(wstring20);
        String::ToUpper(wstring20);
        String::ToLower(wstring21);
        String::ToUpper(wstring21);

        auto hashstring = "b"_hash;
    }
};

}
