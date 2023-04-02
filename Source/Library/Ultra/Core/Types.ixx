export module Ultra.Core.Types;

// Default
export import <algorithm>;
export import <array>;
export import <chrono>;
export import <cstdlib>;
export import <cstdint>;
export import <exception>;
export import <format>;
export import <functional>;
export import <iomanip>;
export import <limits>;
export import <map>;
export import <memory>;
export import <mutex>;
export import <ostream>;
export import <random>;
export import <string>;
export import <string_view>;
export import <type_traits>;
export import <unordered_map>;
export import <vector>;
export import <utility>;

// Extensions
//export import "Private/Types.h";

///
/// @brief The following containers, literals and types are used quite often, therefore they are exposed under the root namespace.
///
export namespace Ultra {

// Container
using std::array;
using std::byte;
using std::map;
using std::string;
using std::wstring;
using std::string_view;
using std::wstring_view;
using std::unordered_map;
using std::vector;

// Smart-Pointer
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

// Streams
using std::format;
using std::ostream;
using std::stringstream;
using std::vformat;

// Threading
using std::mutex;

// Types
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::intptr_t;
using std::size_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uintptr_t;

///
/// @brief The following definitions serve as replacement possibility.
/// 

// Reference Pointer
template<typename T>
using Reference = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Reference<T> CreateReference(Args && ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// Scope Pointer
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args && ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

}
