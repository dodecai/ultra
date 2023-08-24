export module Ultra.Core.Helpers;

import <string_view>;
import <vector>;

///
/// @brief These little helpers can be very useful in some cases.
///

export namespace Ultra {

///
/// Category: Enumerations
///

///
/// @brief Converts enumeration value to BitMask value
///
template<typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>, T>>
constexpr auto BitMask(T x) { return (1 << x); }

///
/// @brief Converts enumeration to base type
///
template<typename E>
constexpr auto GetEnumType(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

///
/// @brief Derived Enums
/// @note  The value is declared as a union mainly for as a debugging aid. If the union is undesired and you have other methods of debugging,
/// change it to either of EnumT and do a cast for the constructor that accepts BaseEnumT.
///
template <typename derivedT, typename baseT>
class DeriveEnum {
    union {
        derivedT Derived;
        baseT Base;
    };

public:
    DeriveEnum() {}
    DeriveEnum(derivedT e): Derived(e) {}
    DeriveEnum(baseT e): Base(e) {}
    explicit DeriveEnum(int val): Derived(static_cast<derivedT>(val)) {}
    operator derivedT() const { return Derived; }
};


///
/// Category: string_view
///

///
/// @brief Compile-Time Variant of rfind in a string view
///
consteval size_t constexpr_rfind(const std::string_view str, char ch, size_t pos = std::string_view::npos) {
    if (pos >= str.size()) { pos = str.size() - 1; }

    for (size_t i = pos; i != static_cast<size_t>(-1); --i) {
        if (str[i] == ch) { return i; }
    }

    return std::string_view::npos;
}


///
/// Category: vector
///

///
/// @brief Vector SizeOf
///
template<typename T>
size_t sizeof_vector(const typename std::vector<T> &vec) {
    return sizeof(T) * vec.size();
}

}
