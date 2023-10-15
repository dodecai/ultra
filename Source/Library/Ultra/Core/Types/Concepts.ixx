export module Ultra.Core.Concepts;

import Ultra.Core.Types;

///
/// @brief Concepts
///

export namespace Ultra {

///
/// @brief Concepts for Aritmetic Types
///

// Detects: All Arithmetic Types
template <typename T>
concept typename_arithmetic = std::is_arithmetic_v<T>;


///
/// @brief Concepts for Floating-Point Types
///

// Detects: Default Floating-Point Types
template <typename T>
concept typename_floating_point = std::is_floating_point_v<T>;


///
/// @brief Concepts for Hasable Types
///

// All size_t convertible Types
template<typename T>
concept typename_hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};


///
/// @brief Alias-Templates for String Types
///

// Detects: All String Types
template<typename T>
using is_xstring = std::disjunction<
    std::is_same<char *, typename std::decay_t<T>>,
    std::is_same<const char *, typename std::decay_t<T>>,
    std::is_same<std::string, typename std::decay_t<T>>,
    std::is_same<std::string_view, typename std::decay_t<T>>,
    std::is_same<wchar_t *, typename std::decay_t<T>>,
    std::is_same<const wchar_t *, typename std::decay_t<T>>,
    std::is_same<std::wstring, typename std::decay_t<T>>,
    std::is_same<std::wstring_view, typename std::decay_t<T>>
>;

template<typename T>
constexpr bool is_xstring_v = is_xstring<T>::value;

// Detects: All String-View Types
template <typename T>
using is_xstring_view = std::disjunction<
    std::is_same<std::string_view, std::decay<T>>,
    std::is_same<std::wstring_view, std::decay<T>>
>;

template<typename T>
constexpr bool is_xstring_view_v = is_xstring_view<T>::value;

// Detects: Default String Types
template<typename T>
using is_string = std::disjunction<
    std::is_same<char *, typename std::decay_t<T>>,
    std::is_same<const char *, typename std::decay_t<T>>,
    std::is_same<std::string, typename std::decay_t<T>>,
    std::is_same<std::string_view, typename std::decay_t<T>>
>;

template<typename T>
constexpr bool is_string_v = is_string<T>::value;

// Detects: Wide String Types
template<typename T>
using is_wstring = std::disjunction<
    std::is_same<wchar_t *, typename std::decay_t<T>>,
    std::is_same<const wchar_t *, typename std::decay_t<T>>,
    std::is_same<std::wstring, typename std::decay_t<T>>,
    std::is_same<std::wstring_view, typename std::decay_t<T>>
>;

template<typename T>
constexpr bool is_wstring_v = is_wstring<T>::value;


///
/// @brief Concepts for String Types
///

// All String Types
template<typename T>
concept typename_xstring = is_xstring_v<T>;

// All String-View Types
template<typename T>
concept typename_string_view = is_xstring_view_v<T>;

// Default String Types
template<typename T>
concept typename_string = is_string_v<T>;

// Wide String Types
template<typename T>
concept typename_wstring = is_wstring_v<T>;

}
