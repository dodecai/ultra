export module Ultra.Utility.Vector;

import Ultra.Core;

export namespace Ultra {

template<typename T>
size_t vectorsizeof(const typename std::vector<T> &vec) {
    return sizeof(T) * vec.size();
}


}
