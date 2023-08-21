export module Ultra.Core.Concepts;

import <type_traits>;

export namespace Ultra {

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

}
