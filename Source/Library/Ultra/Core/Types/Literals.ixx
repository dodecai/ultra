export module Ultra.Core.Literals;

import <string_view>;

///
/// @brief Literals
///

export namespace Ultra {

///
/// @brief Hashes a string literal.
/// @detail This custom literal operator hashes the provided string literal.
/// 
/// @example
/// Usage: "This is my string literal"_hash
///
constexpr inline size_t operator"" _hash(const char *value, size_t count) {
    if (count > 0) {
        return !value ? 0 : std::hash<std::string_view> {}(std::string_view(value, count));
    }
    return 0;
}
///
/// @brief Hashes a wide string literal.
/// @detail This custom literal operator hashes the provided wide string literal.
/// 
/// @example
/// Usage: L"This is my wide string literal"_hash
///
constexpr inline size_t operator"" _hash(const wchar_t *value, size_t count) {
    if (count > 0) {
        return !value ? 0 : std::hash<std::wstring_view> {}(std::wstring_view(value, count));
    }
    return 0;
}

}
