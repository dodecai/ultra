#pragma once

#include "Omnia/Omnia.pch"

namespace Omnia {

/**
* @brief Class: Removes the copy constructer/operator and from derived classes.
*/
class NonCopyable {
protected:
    NonCopyable() = default;
    virtual ~NonCopyable() = default;

public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable(NonCopyable &&) noexcept = default;
    NonCopyable &operator=(const NonCopyable &) = delete;
    NonCopyable &operator=(NonCopyable &&) noexcept = default;
};

/**
* @brief Property: Built-In Getter and Setter for <any> type
*/
template<typename T>
class Property {
	T Value;
public:
	Property(T value):	Value {value} {};

	T &operator=(const T &value) { return Value = value; };
	operator T const &() const { return Value; };
};

/**
* @brief Property: Built-In Getter and Setter for <arithmetic> types with clamping 'from # to # in steps #' feature
*/
template <typename T>
class ArithmeticProperty {
	static_assert(std::is_arithmetic<T>::value);
	T Value = 0;
	T Minimum = 0;
	T Maximum = 0;
	T Step = 0;

public:
	// Predefined values {0}
	ArithmeticProperty() = default;
	// Define min and max (value = max)
	ArithmeticProperty(T min, T max): Value {max}, Minimum {min}, Maximum {max} {};
	// Define value, min and max
	ArithmeticProperty(T value, T min, T max): Value {value}, Minimum {min}, Maximum {max} {};
	
	T &operator=(const T &value) {
		// ToDo: Implement 'in steps #' feature
		if (!Minimum && !Maximum) {
			return Value;
		} else {
			return Value = std::clamp(value, Minimum, Maximum);
		}
	};
	operator T const &() const {
		return Value;
	}
};

}
