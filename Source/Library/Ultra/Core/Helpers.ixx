export module Ultra.Core.Helpers;

import Ultra.Core.Types;

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
/// @brief Decodes UTF8 string view to codepoint 
///
inline uint32_t DecodeUtf8(string_view::iterator &begin, string_view::iterator end) {
    unsigned char byte = *begin;
    uint32_t codepoint = 0;
    int additionalBytes = 0;

    // 1-Byte Character (ASCII)
    if (byte <= 0x7F) {
        codepoint = byte;
    // 2-Byte Character
    } else if ((byte & 0xE0) == 0xC0) {
        codepoint = byte & 0x1F;
        additionalBytes = 1;
    // 3-Byte Character
    } else if ((byte & 0xF0) == 0xE0) {
        codepoint = byte & 0x0F;
        additionalBytes = 2;
    // 4-Byte Character
    } else if ((byte & 0xF8) == 0xF0) {
        codepoint = byte & 0x07;
        additionalBytes = 3;
    // Invalid UTF-8 start byte
    } else {
        begin++;
        return 0xFFFD; // Unicode replacement character
    }

    begin++;

    while (additionalBytes > 0) {
        if (begin == end || (*begin & 0xC0) != 0x80) {
            // Premature end or invalid UTF-8 continuation byte
            return 0xFFFD; // Unicode replacement character
        }

        codepoint = (codepoint << 6) | (*begin & 0x3F);
        begin++;
        additionalBytes--;
    }

    return codepoint;
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
