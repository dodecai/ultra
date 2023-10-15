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
/// [    4    |  random generated        ]  -> Currently Supported
/// [    5    |  name-based (SHA-1)      ]
/// --------------------------------------
/// Structure:
/// uint128_t    = uint32_t - uint16_t  - uint16_t  - (uint8_t + uint8_t)   - uint48_t
/// 32           = 8        - 4         - 4         - 4                     - 12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma endregion

// Module
export module Ultra.Core.UUID;

// Library
import Ultra.Core.Types;

export namespace Ultra {

///
/// @brief Currently we use a string to hold the 128-Bit value, cause in our usecase it doesn't really matter
/// All other unsigned integers hold a basic random number, but we are ready for the future, if a build-in 128-Bit sized integer arrives.
/// 
template<typename T>
concept typename_uuid = std::is_same_v<string, T> || (std::is_unsigned_v<T> && sizeof(T) <= 16);

///
/// @brief Universally Unique Identifier (v4: randomly generated)
/// @tparam <string>:   128-Bit (full UUID, default)
/// @tparam <uintN_t>:  [16|32|64]-Bit (partial UUID, based on type size)
///
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

    // Comparisions
    bool operator==(const T &rhs) const { return Value == rhs; }
    bool operator!=(const T &rhs) const { return Value != rhs; }
    bool operator==(const UUID &rhs) const { return Value == rhs.Value; }
    bool operator!=(const UUID &rhs) const { return Value != rhs.Value; }

    // Stream Support
    template<typename T>
    friend ostream &operator <<(ostream &os, const UUID<T> &rhs);

private:
    // Methods
    void Generate() {
        if constexpr (std::is_unsigned_v<T>) {
            // >128-Bit with random value (type dependent)
            std::uniform_int_distribution<T> distribution { 0, std::numeric_limits<T>::max() };
            Value = distribution(sGenerator);
        } else {
            // 128-Bit with random value
            std::uniform_int_distribution<uint64_t> distribution { 0, std::numeric_limits<uint64_t>::max() };
            array<uint8_t, 16> bytes;
            for (size_t i = 0; i < 16; i += 8) {
                uint64_t value = distribution(sGenerator);
                std::memcpy(bytes.data() + i, &value, sizeof(value));
            }

            // Update UUID field
            bytes[6] = (bytes[6] & 0x0f) | 0x40;  // Version 4
            bytes[8] = (bytes[8] & 0x3f) | 0x80;  // Variant RFC 4122
            
            // Convert bytes to string format
            std::stringstream result;
            result << std::hex << std::setfill('0');
            
            for (int i = 0; i < 16; i++) {
                if (i == 4 || i == 6 || i == 8 || i == 10) {
                    result << '-';
                }
                result << std::setw(2) << static_cast<int>(bytes[i]);
            }
            Value = result.str();
        }
    }

private:
    // Properties
    T Value;
    static inline std::random_device sRandomDevice;
    static inline std::mt19937_64 sGenerator { sRandomDevice() };
};

// Allow ostream to ouptut UUIDs without conversations
template<typename T>
ostream &operator <<(ostream &os, const UUID<T> &rhs) {
    os << rhs.Value;
    return os;
}

}
