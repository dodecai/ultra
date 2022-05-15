#pragma once

#pragma region Information

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Universally Unique Identifier (UUID)
/// Sources:    https://tools.ietf.org/html/rfc4122
///             https://en.wikipedia.org/wiki/Universally_unique_identifier
/// 
/// [ Version |  Description             ]
/// --------------------------------------
/// [    1    |  timestamp based         ]
/// [    2    |  DCE security version    ]
/// [    3    |  name-based (MD5)        ]
/// [    4    |  random generated        ]
/// [    5    |  name-based (SHA-1)      ]
/// --------------------------------------
/// Structure:
/// uint128_t    = uint32_t - uint16_t  - uint16_t  - (uint8_t + uint8_t)   - uint48_t
/// 32           = 8        - 4         - 4         - 4                     - 12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma endregion

#include "Omnia/Core.h"

namespace Omnia {

// Currently we use string to hold the 128-Bit value, cause in our usecase it doesn't really matter, all other unsigned integers hold a basic random number
template<typename T>
concept typename_uuid = std::is_same_v<string, T> || std::is_unsigned_v<T>;

/// @brief Universally Unique Identifier (v4: randomly generated)
/// @param <string>:    128-Bit (full UUID, default)
/// @param <uintN_t>:   [8|16|32|64]-Bit (partial UUID, based on type size)
template<typename_uuid T = string>
class UUID {
public:
    // Default
    UUID() { Generate(); }
    UUID(const T &value): Value(value) {}
    UUID(const UUID &other): Value(other.Value) {}
    UUID(UUID &&other): Value(std::move(other.Value)) {}
    ~UUID() = default;

    // Accessors
    operator T() { return Value; }
    operator UUID() { return *this; }
    
    // Mutators
    T &operator=(const T &rhs) { Value = rhs; return Value; }
    UUID &operator=(const UUID &rhs) { Value = rhs.Value; return *this; }
    UUID &operator=(UUID &&rhs) { Value = std::move(rhs.Value); return *this; }

    // Comparision
    bool operator==(const T &rhs) { return (Value == rhs); }
    bool operator!=(const T &rhs) { return (Value != rhs); }
    bool operator==(const UUID &rhs) { return (Value == rhs.Value); }
    bool operator!=(const UUID &rhs) { return (Value != rhs.Value); }

    // Operators
    template<typename T>
    friend ostream &operator <<(ostream &os, const UUID<T> &rhs);

private:
    // Internal
    void Generate() {
        if constexpr (std::is_unsigned_v<T>) {
            Value = sUniformDistribution(sEngine);
        } else {
            stringstream result;
            result << std::hex;
            for (int i = 0; i < 8; i++) result << sUniformDistribution(sEngine, 15);
            result << "-";
            for (int i = 0; i < 4; i++) result << sUniformDistribution(sEngine, 15);
            result << "-";
            for (int i = 0; i < 4; i++) result << sUniformDistribution(sEngine, 15);
            result << "-";
            for (int i = 0; i < 4; i++) result << sUniformDistribution(sEngine, 15);
            result << "-";
            for (int i = 0; i < 12; i++) result << sUniformDistribution(sEngine, 15);
            Value = result.str();

            // Alternative: Every position is randomly generated
            //static const char *ValidChars = "0123456789abcdef";
            //static array<uint8_t, 36> DashPositions = {
            //    0, 0, 0, 0, 0, 0, 0, 0, 1,
            //    0, 0, 0, 0, 1,
            //    0, 0, 0, 0, 1,
            //    0, 0, 0, 0, 1,
            //    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            //};
            //for (int i = 0; i < 36; i++) {
            //    mIDC += (DashPositions[i] == 1) ? '-' : ValidChars[(sUniformDistributionShort(sEngine) % 16)];
            //}
        }
    }

private:
    // Properties
    T Value;

    static inline std::random_device sRandomDevice;
    static inline std::mt19937_64 sEngine { sRandomDevice() };
    static inline std::uniform_int_distribution<size_t> sUniformDistribution;
};

// Allow ostream to ouptut UUIDs without conversations
template<typename T>
ostream &operator <<(ostream &os, const UUID<T> &rhs) {
    os << rhs.Value;
    return os;
}

}
