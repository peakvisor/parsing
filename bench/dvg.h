#ifndef BENCH_LAB_DVG_H
#define BENCH_LAB_DVG_H

#include <iostream>
#include <fstream>

#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <algorithm>

#define CONCAT_INTERNAL(X, Y) X##Y
#define CONCAT(X, Y) CONCAT_INTERNAL(X, Y)

struct StringView {
    const char *begin, *end;
    StringView(const char *begin, const char *end) : begin{begin}, end{end} {}
    StringView(const char *begin, size_t length) : begin{begin}, end{begin + length} {}
    StringView(const char *begin, int length) : begin{begin}, end{begin + length} {}
    StringView(const char *begin) : StringView{begin, std::strlen(begin)} {}

    inline int length() const { return static_cast<int>(end - begin); }

    explicit StringView(const std::string_view &string)
        : StringView{string.data(), string.size()}
    {} // not sure if safe to be implicit

    // TODO: khm... unsafe
    operator std::string_view () const {
        return {begin, static_cast<size_t>(length())};
    }
};

template <typename Value>
struct DVGKeyPair {
    Value value;
    const char *key;
    int keyLength;

    template <int keyCapacity>
    constexpr DVGKeyPair(const Value &value, const char (&key)[keyCapacity])
        : value{value}, key{key}, keyLength{keyCapacity - 1}
    {}

    inline bool equals(const char *other, int otherLength) const {
        return keyLength == otherLength && std::strncmp(other, key, keyLength) == 0;
    }

    template <typename Walue = Value, int count>
    static inline Walue valueForKey(const StringView &key, const DVGKeyPair (&mapping)[count], Walue fallback = {}) {
        auto length = key.length();
        for (auto &variant : mapping) {
            if (variant.equals(key.begin, length)) { return variant.value; }
        }
        return fallback;
    }

    template <int count>
    static inline const char *keyForValue(const Value &value, const DVGKeyPair (&mapping)[count], const char *fallback = "") {
        for (auto &variant : mapping) {
            if (value == variant.value) { return variant.key; }
        }
        return fallback;
    }
};

template <typename T, typename = void>
struct hasDecode : std::false_type {};

template <typename T>
struct hasDecode<T, decltype(void(&T::decode))> : std::true_type {};

template <typename Mapping, typename Enum = typename Mapping::E>
inline __attribute__((always_inline)) Enum decode(std::string_view string) {
    if constexpr (hasDecode<Mapping>::value) {
        return Mapping::decode(string);
    } else {
        return DVGKeyPair<Enum>::valueForKey(string, Mapping::values, Enum{});
    }
}

template <typename Mapping, typename Enum = typename Mapping::E>
struct SwitchDecoder {
    static inline Enum decode(const std::string_view &view) {
        return ::decode<Mapping>(StringView(&view.front(), view.size()));
    }
};

template <typename Mapping, typename Enum = typename Mapping::E>
struct DefaultDecoder {
    static inline Enum decode(const std::string_view &string) {
        return DVGKeyPair<Enum>::valueForKey(StringView{string}, Mapping::values, Enum{});
    }
};

#endif //BENCH_LAB_DVG_H
