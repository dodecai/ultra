export module Ultra.CoreNew;

// Default Library
export import std;

// Custom Library
export import "Core.h";
export import "Platform.h";
export import Ultra.Core.Helpers;

export import Ultra.Core.Arguments;
export import Ultra.Core.Concepts;
export import Ultra.Core.Literals;
export import Ultra.Core.Object;
export import Ultra.Core.Property;
export import Ultra.Core.SmartPointer;
export import Ultra.Core.SourceLocation;
export import Ultra.Core.Timestamp;
export import Ultra.Core.UUID;

export namespace Ultra {

// Concurrency
using std::atomic;
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
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::wostringstream;
using std::stringstream;
using std::vformat;
using std::wofstream;
using std::wostream;

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

}
