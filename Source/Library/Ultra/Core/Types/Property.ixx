export module Ultra.Core.Property;

import <algorithm>;
import <functional>;

export namespace Ultra {

///
/// @brief Property: Built-In Getter and Setter for <any> type
///
/// @example
/// Ultra::Property<size_t> Size { 0, [this](const size_t &value) {
///    return value == 1000 ? 0 : value;
/// }};
///
template <typename T>
class Property {
    // Types
    using Setter = std::function<T(const T &)>;

public:
    Property(): mValue(T()), mSetter(nullptr) {}
    Property(T value, const Setter &setter = nullptr): mValue(value), mSetter(setter) {}
    
    operator T const &() const { return mValue; }
    Property<T> &operator=(const T &value) {
        mValue = mSetter ? mSetter(value) : value;
        return *this;
    }

private:
    T mValue;
    Setter mSetter;
};

///
/// @brief Property: Built-In Getter and Setter for <arithmetic> types with clamping 'from # to # in steps #' feature
///
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
    operator T const &() const { return Value; }
};

}
