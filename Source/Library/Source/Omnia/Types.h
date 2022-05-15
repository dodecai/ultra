#pragma once

#include "Omnia.pch"

namespace Omnia {

using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

using std::array;
using std::function;
using std::future;
using std::map;
using std::optional;
using std::shared_ptr;
using std::string;
using std::string_view;
using std::stringstream;
using std::unordered_map;
using std::unique_ptr;
using std::vector;
using std::weak_ptr;

using namespace std::literals::chrono_literals;
using namespace std::string_literals;

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
