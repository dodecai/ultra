export module Ultra.Core.SmartPointer;

import <memory>;

///
/// @brief The following definitions serve as replacement possibility.
/// 

export namespace Ultra {

// Reference Pointer
template<typename T>
using Reference = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Reference<T> CreateReference(Args && ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// Reference View Pointer
template<typename T>
using ReferenceView = std::weak_ptr<T>;

template<typename T, typename ... Args>
constexpr ReferenceView<T> CreateReferenceView(const Reference<T> &reference) {
    return ReferenceView<T>(reference);
}

// Scope Pointer
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args && ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

}
