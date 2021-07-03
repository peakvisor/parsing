#ifndef BENCH_LAB_GEO_RESOURCE_TYPE_H
#define BENCH_LAB_GEO_RESOURCE_TYPE_H
#include "dvg.h"
#include "bench_common.h"

namespace ResourceTypeEnumTesting {

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

struct ResourceTypeMapping {
    using E = DVGGeoResource::Type;
    static constexpr DVGKeyPair<DVGGeoResource::Type> values[]{
        {DVGGeoResource::Type::kDEM, "DEM"},
        {DVGGeoResource::Type::kHDDEM, "HDDEM"},
        {DVGGeoResource::Type::kTEX, "TEX"},
        {DVGGeoResource::Type::kGDB, "GDB"},
        {DVGGeoResource::Type::kTRL, "TRL"},

        {DVGGeoResource::Type::kHDSAT, "HDSAT"},

        {DVGGeoResource::Type::kSAT, "SAT"},
        {DVGGeoResource::Type::kLSAT, "LSAT"},
        {DVGGeoResource::Type::kLTEX, "LTEX"},
        {DVGGeoResource::Type::kLDEM, "LDEM"},

        {DVGGeoResource::Type::kHDWSAT, "HDWSAT"},
        {DVGGeoResource::Type::kWSAT, "WSAT"},
        {DVGGeoResource::Type::kLWSAT, "LWSAT"},
    };

    static inline E decode(std::string_view string) {
        using T = ResourceTypeMapping::E;
        if (string.size() >= 3) {
            switch (string[2]) {
                case 'A': return string[0] == 'L' ?
                        (string == "LSAT" ? T::kLSAT : T{}):
                        string == "WSAT" ? T::kWSAT : T{};
                case 'B': return string == "GDB" ? T::kGDB : T{};
                case 'D': return string == "HDDEM" ? T::kHDDEM : T{};
                case 'E': return string[1] == 'T' ?
                        (string == "LTEX" ? T::kLTEX : T{}):
                        string == "LDEM" ? T::kLDEM : T{};
                case 'L': return string == "TRL" ? T::kTRL : T{};
                case 'M': return string == "DEM" ? T::kDEM : T{};
                case 'S': return string[1] == 'D' ?
                        (string == "HDSAT" ? T::kHDSAT : T{}) :
                        string == "LWSAT" ? T::kLWSAT : T{};
                case 'T': return string == "SAT" ? T::kSAT : T{};
                case 'W': return string == "HDWSAT" ? T::kHDWSAT : T{};
                default: return string == "TEX" ? T::kTEX : T{};
            }
        }
        return {};
    }
};

using T = DVGGeoResource::Type;
static auto resourceTypesString = generateTestStrings<ResourceTypeMapping>();

struct FrozenByFirstThree {
    static constexpr
    frozen::unordered_map<frozen::string, DVGGeoResource::Type, 13, FirstThreeElsa> map {
        {"DEM", DVGGeoResource::Type::kDEM},
        {"HDDEM", DVGGeoResource::Type::kHDDEM},
        {"TEX", DVGGeoResource::Type::kTEX},
        {"GDB", DVGGeoResource::Type::kGDB},
        {"TRL", DVGGeoResource::Type::kTRL},
        {"HDSAT", DVGGeoResource::Type::kHDSAT},
        {"SAT", DVGGeoResource::Type::kSAT},
        {"LSAT", DVGGeoResource::Type::kLSAT},
        {"LTEX", DVGGeoResource::Type::kLTEX},
        {"LDEM", DVGGeoResource::Type::kLDEM},
        {"LWSAT", DVGGeoResource::Type::kLWSAT},
        {"WSAT", DVGGeoResource::Type::kWSAT},
        {"HDWSAT", DVGGeoResource::Type::kHDWSAT}
    };

    static inline DVGGeoResource::Type decode(const std::string_view &view) {
        auto it = map.find(frozen::string(&view.front(), view.size()));
        if (it == map.end()) {
            return DVGGeoResource::Type{};
        }
        return it->second;
    }
};

struct FrozenDefault {
    static constexpr
    frozen::unordered_map<frozen::string, DVGGeoResource::Type, 13> map {
        {"DEM", DVGGeoResource::Type::kDEM},
        {"HDDEM", DVGGeoResource::Type::kHDDEM},
        {"TEX", DVGGeoResource::Type::kTEX},
        {"GDB", DVGGeoResource::Type::kGDB},
        {"TRL", DVGGeoResource::Type::kTRL},
        {"HDSAT", DVGGeoResource::Type::kHDSAT},
        {"SAT", DVGGeoResource::Type::kSAT},
        {"LSAT", DVGGeoResource::Type::kLSAT},
        {"LTEX", DVGGeoResource::Type::kLTEX},
        {"LDEM", DVGGeoResource::Type::kLDEM},
        {"LWSAT", DVGGeoResource::Type::kLWSAT},
        {"WSAT", DVGGeoResource::Type::kWSAT},
        {"HDWSAT", DVGGeoResource::Type::kHDWSAT}
    };

    static inline DVGGeoResource::Type decode(const std::string_view &view) {
        auto it = map.find(frozen::string(&view.front(), view.size()));
        if (it == map.end()) {
            return DVGGeoResource::Type{};
        }
        return it->second;
    }
};

struct SwitchThirdWOVerification {
    static inline DVGGeoResource::Type decode(const std::string_view &string) {

        if (string.size() >= 3) {
            switch (string[2]) {
                case 'A': return string[0] == 'L' ? T::kLSAT : T::kWSAT;
                case 'B': return T::kGDB;
                case 'D': return T::kHDDEM;
                case 'E': return string[1] == 'T' ? T::kLTEX : T::kLDEM;
                case 'L': return T::kTRL;
                case 'M': return T::kDEM;
                case 'S': return string[1] == 'D' ? T::kHDSAT : T::kLWSAT;
                case 'T': return T::kSAT;
                case 'W': return T::kHDWSAT;
                case 'X': return T::kTEX;
                default: return T::kUnknown;
            }
        }
        return {};
    }
};

struct SwitchThird {
    static inline DVGGeoResource::Type decode(const std::string_view &string) {
        if (string.size() >= 3) {
            switch (string[2]) {
                case 'A': return string[0] == 'L' ?
                        (string == "LSAT" ? T::kLSAT : T{}):
                        string == "WSAT" ? T::kWSAT : T{};
                case 'B': return string == "GDB" ? T::kGDB : T{};
                case 'D': return string == "HDDEM" ? T::kHDDEM : T{};
                case 'E': return string[1] == 'T' ?
                        (string == "LTEX" ? T::kLTEX : T{}):
                        string == "LDEM" ? T::kLDEM : T{};
                case 'L': return string == "TRL" ? T::kTRL : T{};
                case 'M': return string == "DEM" ? T::kDEM : T{};
                case 'S': return string[1] == 'D' ?
                        (string == "HDSAT" ? T::kHDSAT : T{}) :
                        string == "LWSAT" ? T::kLWSAT : T{};
                case 'T': return string == "SAT" ? T::kSAT : T{};
                case 'W': return string == "HDWSAT" ? T::kHDWSAT : T{};
                default: return string == "TEX" ? T::kTEX : T{};
            }
        }
        return {};
    }
};

//struct SwitchSize {
//    static inline DVGGeoResource::Type decode(const std::string_view &string) {
//        switch (string.size()) {
//            case 3:
//                switch (string[2]) {
//                    case 'M': return string == "DEM" ? T::kDEM : T{};
//                    case 'X': return string == "TEX" ? T::kTEX : T{};
//                    case 'B': return string == "GDB" ? T::kGDB : T{};
//                    case 'L': return string == "TRL" ? T::kTRL : T{};
//                    case 'T': return string == "SAT" ? T::kSAT : T{};
//                }
//           case 4:
//               switch (string[3]) {
//                   case
//               }
//        }
//    }
//};
struct ResourceType {
    using Mapping = ResourceTypeMapping;

    static inline std::vector<std::string_view>& views() {
        return resourceTypesString.views;
    }
};

#define RESOURCE_TYPE_BENCHMARK(Decoder) VERIFIED_BENCHMARK(Decoder, ResourceType);

RESOURCE_TYPE_BENCHMARK(SwitchDecoder<ResourceTypeMapping>);
RESOURCE_TYPE_BENCHMARK(DefaultDecoder<ResourceTypeMapping>);
RESOURCE_TYPE_BENCHMARK(FrozenByFirstThree);
RESOURCE_TYPE_BENCHMARK(FrozenDefault);
RESOURCE_TYPE_BENCHMARK(SwitchThirdWOVerification);
RESOURCE_TYPE_BENCHMARK(SwitchThird);

} // namespace


#endif //BENCH_LAB_GEO_RESOURCE_TYPE_H
