#ifndef BENCH_LAB_GEO_RESOURCE_FORMAT_H
#define BENCH_LAB_GEO_RESOURCE_FORMAT_H

#include "dvg.h"
#include "bench_common.h"

namespace ResourceFormatEnumTesting {

struct DVGGeoResource {
    enum class Type : uint16_t {
        kUnknown = 0,
        kDEM,
        kHDDEM,
        kTEX,
        kGDB,
        kTRL,

        kHDSAT,

        kSAT,
        kLSAT,
        kLTEX,
        kLDEM,

        kHDWSAT,
        kWSAT,
        kLWSAT,

        kRESOURCE_TYPE_COUNT
    };

    enum class Format : uint16_t {
        kEmpty,
        kRaw,
        kM2SEP,
        kJPEG2000,
        kDFRAC,
        kGDB,
        kTRL,
        kHEIC
    };
};

struct ResourceFormatMapping {
    using E = DVGGeoResource::Format;
    static constexpr DVGKeyPair<DVGGeoResource::Format> values[]{
        {E::kEmpty, "EMPTY"},
        {E::kRaw, "RAW"},
        {E::kM2SEP, "M2SEP"},
        {E::kJPEG2000, "J2K"},
        {E::kDFRAC, "DFRAC"},
        {E::kGDB, "GDB"},
        {E::kTRL, "TRL"},
        {E::kHEIC, "HEIC"},
    };

    static  inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() >= 3) {
            switch (string[0]) {
                case 'D': return string == "DFRAC" ? E::kDFRAC : E{};
                case 'E': return string == "EMPTY" ? E::kEmpty : E{};
                case 'G': return string == "GDB" ? E::kGDB : E{};
                case 'H': return string == "HEIC" ? E::kHEIC : E{};
                case 'J': return string == "J2K" ? E::kJPEG2000 : E{};
                case 'M': return string == "M2SEP" ? E::kM2SEP : E{};
                case 'R': return string == "RAW" ? E::kRaw : E{};
                default: return string == "TRL" ? E::kTRL : E{};
            }
        }
        return {};
    }
};

using T = DVGGeoResource::Format;
static auto ResourceFormatsString = generateTestStrings<ResourceFormatMapping>();

struct FrozenByFirst {
    static constexpr
    frozen::unordered_map<frozen::string, DVGGeoResource::Format, 8, FirstElsa> map {
        {"EMPTY", T::kEmpty},
        {"RAW", T::kRaw},
        {"M2SEP", T::kM2SEP},
        {"J2K", T::kJPEG2000},
        {"DFRAC", T::kDFRAC},
        {"GDB", T::kGDB},
        {"TRL", T::kTRL},
        {"HEIC", T::kHEIC},
    };

    static inline DVGGeoResource::Format decode(const std::string_view &view) {
        auto it = map.find(frozen::string(&view.front(), view.size()));
        if (it == map.end()) {
            return DVGGeoResource::Format{};
        }
        return it->second;
    }
};

struct FrozenByFirstThree {
    static constexpr
    frozen::unordered_map<frozen::string, DVGGeoResource::Format, 8, FirstThreeElsa> map {
        {"EMPTY", T::kEmpty},
        {"RAW", T::kRaw},
        {"M2SEP", T::kM2SEP},
        {"J2K", T::kJPEG2000},
        {"DFRAC", T::kDFRAC},
        {"GDB", T::kGDB},
        {"TRL", T::kTRL},
        {"HEIC", T::kHEIC},
    };

    static inline DVGGeoResource::Format decode(const std::string_view &view) {
        auto it = map.find(frozen::string(&view.front(), view.size()));
        if (it == map.end()) {
            return DVGGeoResource::Format{};
        }
        return it->second;
    }
};

struct FrozenDefault {
    static constexpr
    frozen::unordered_map<frozen::string, DVGGeoResource::Format, 8> map {
        {"EMPTY", T::kEmpty},
        {"RAW", T::kRaw},
        {"M2SEP", T::kM2SEP},
        {"J2K", T::kJPEG2000},
        {"DFRAC", T::kDFRAC},
        {"GDB", T::kGDB},
        {"TRL", T::kTRL},
        {"HEIC", T::kHEIC},
    };

    static inline DVGGeoResource::Format decode(const std::string_view &view)
    {
        auto it = map.find(frozen::string(&view.front(), view.size()));
        if (it == map.end()) { return DVGGeoResource::Format{}; }
        return it->second;
    }
};

struct SwitchFirst {
    static inline DVGGeoResource::Format decode(const std::string_view &string) {
        if (string.size() >= 3) {
            switch (string[0]) {
                case 'D': return string == "DFRAC" ? T::kDFRAC : T{};
                case 'E': return string == "EMPTY" ? T::kEmpty : T{};
                case 'G': return string == "GDB" ? T::kGDB : T{};
                case 'H': return string == "HEIC" ? T::kHEIC : T{};
                case 'J': return string == "J2K" ? T::kJPEG2000 : T{};
                case 'M': return string == "M2SEP" ? T::kM2SEP : T{};
                case 'R': return string == "RAW" ? T::kRaw : T{};
                default: return string == "TRL" ? T::kTRL : T{};
            }
        }
        return {};
    }
};

struct SwitchFirstMessedOrder {
    static inline DVGGeoResource::Format decode(const std::string_view &string) {
        if (string.size() >= 3) {
            switch (string[0]) {
                case 'G': return string == "GDB" ? T::kGDB : T{};
                case 'M': return string == "M2SEP" ? T::kM2SEP : T{};
                case 'J': return string == "J2K" ? T::kJPEG2000 : T{};
                case 'R': return string == "RAW" ? T::kRaw : T{};
                case 'H': return string == "HEIC" ? T::kHEIC : T{};
                case 'E': return string == "EMPTY" ? T::kEmpty : T{};
                case 'D': return string == "DFRAC" ? T::kDFRAC : T{};
                default: return string == "TRL" ? T::kTRL : T{};
            }
        }
        return {};
    }
};

struct SwitchFirstMessedDefault {
    static  inline __attribute__((always_inline)) DVGGeoResource::Format decode(const std::string_view &string) {
        if (string.size() >= 3) {
            switch (string[0]) {
                case 'D': return string == "DFRAC" ? T::kDFRAC : T{};
                case 'E': return string == "EMPTY" ? T::kEmpty : T{};
                case 'G': return string == "GDB" ? T::kGDB : T{};
                case 'J': return string == "J2K" ? T::kJPEG2000 : T{};
                case 'M': return string == "M2SEP" ? T::kM2SEP : T{};
                case 'R': return string == "RAW" ? T::kRaw : T{};
                case 'T': return string == "TRL" ? T::kTRL : T{};
                default: return string == "HEIC" ? T::kHEIC : T{};
            }
        }
        return {};
    }
};

struct ResourceFormat {
    using Mapping = ResourceFormatMapping;

    static inline std::vector<std::string_view> &views() {
        return ResourceFormatsString.views;
    }
};

#define RESOURCE_FORMAT_BENCHMARK(Decoder) VERIFIED_BENCHMARK(Decoder, ResourceFormat);

RESOURCE_FORMAT_BENCHMARK(SwitchFirstMessedOrder);
RESOURCE_FORMAT_BENCHMARK(SwitchFirstMessedDefault);
RESOURCE_FORMAT_BENCHMARK(ProdDecoder<ResourceFormatMapping>);
RESOURCE_FORMAT_BENCHMARK(DefaultDecoder<ResourceFormatMapping>);
RESOURCE_FORMAT_BENCHMARK(FrozenByFirst);
RESOURCE_FORMAT_BENCHMARK(FrozenByFirstThree);
RESOURCE_FORMAT_BENCHMARK(FrozenDefault);
RESOURCE_FORMAT_BENCHMARK(SwitchFirst);


} // namespace

#endif //BENCH_LAB_GEO_RESOURCE_FORMAT_H
