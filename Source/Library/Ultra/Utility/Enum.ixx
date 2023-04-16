export module Ultra.Utility.Enum;

import Ultra.Core;

export namespace Ultra {

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
/// @brief Derivered Enums
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

}
