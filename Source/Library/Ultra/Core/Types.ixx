﻿export module Ultra.Core.Types;

// Default
//export import std;
export import <algorithm>;
export import <array>;
export import <atomic>;
export import <bitset>;
export import <chrono>;
export import <condition_variable>;
export import <cstdlib>;
export import <cstdint>;
export import <exception>;
export import <format>;
export import <functional>;
export import <future>;
export import <iomanip>;
export import <iostream>;
export import <limits>;
export import <list>;
export import <map>;
export import <memory>;
export import <mutex>;
export import <ostream>;
export import <queue>;
export import <random>;
export import <ranges>;
export import <source_location>;
export import <stdexcept>;
export import <stack>;
export import <string>;
export import <string_view>;
export import <thread>;
export import <type_traits>;
export import <unordered_map>;
export import <unordered_set>;
export import <vector>;
export import <utility>;

export import Ultra.Core.Arguments;
export import Ultra.Core.Concepts;
export import Ultra.Core.Object;
export import Ultra.Core.Property;
export import Ultra.Core.SmartPointer;
export import Ultra.Core.Timestamp;
export import Ultra.Core.UUID;

///
/// @brief The following containers, literals and types are used quite often, therefore they are exposed under the root namespace.
///
export namespace Ultra {

// Concurrency
using std::condition_variable;
using std::future;
using std::mutex;
using std::thread;

// Container
using std::array;
using std::byte;
using std::function;
using std::list;
using std::map;
using std::queue;
using std::string;
using std::string_view;
using std::unordered_map;
using std::vector;
using std::wstring;
using std::wstring_view;

// Smart-Pointer
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

// Streams
using std::format;
using std::ostream;
using std::stringstream;
using std::vformat;

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

// Literals
using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

}
