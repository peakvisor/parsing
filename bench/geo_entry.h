#ifndef BENCH_LAB_GEO_ENTRY_H
#define BENCH_LAB_GEO_ENTRY_H

#include "dvg.h"
#include "bench_common.h"

namespace EntryTypeEnumTesting {

struct DVGGeoDatabaseEntry {
    enum class Type : uint16_t {
        kUnknown = 0,

        kPeak,
        kCastle,
        kMountainHut,

        kViewpoint,
        kParking,

        kPiste, // TODO: not used and not going to?
        kFunicular,

        kPlace, // i.e. village/town/city
        kGlacier,
        kLake,
        kPass,

        kHotel, // TODO: temporary here?
        kAmenity, // TODO: not fully implemented // shall factor into -> picnic_table/cave_entrance/shelter/water
        kWaterfall, // TODO: not fully implemented
        kWaymark, // TODO: not fully implemented

        kCustomPoint,
        kUserLocation,
        kSun, kMoon, // TODO: debug

        COUNT
    };
};

struct EntryTypeMapping {
    using E = DVGGeoDatabaseEntry::Type;
    static constexpr DVGKeyPair<E> values[]{
        {E::kMountainHut, "alpineHut"},
        {E::kAmenity, "amenity"}, //!
        {E::kCastle, "castle"},
        {E::kViewpoint, "viewpoint"},
        {E::kParking, "parking"},
        {E::kFunicular, "funicularStation"},
        {E::kWaterfall, "waterfall"},

        {E::kPlace, "place"},
        {E::kGlacier, "glacier"},
        {E::kLake, "lake"},
        {E::kPass, "pass"},

        {E::kPeak, "peak"}, // khm. used by d__bookmarkedEntriesCache only
    };

    static inline E decode(std::string_view string) {
        using T = EntryTypeMapping::E;
        switch (string.size()) {
            case 4:
                switch (string[2]) {
                    case 'k':
                        return !std::memcmp(string.begin(), "lake", 4) ? T::kLake : T{};
                    case 's':
                        return !std::memcmp(string.begin(), "pass", 4) ? T::kPass : T{};
                    default:
                        return !std::memcmp(string.begin(), "peak", 4) ? T::kPeak
                            : T{}; //case 'a'
                }
            case 5:
                return !std::memcmp(string.begin(), "place", 5) ? T::kPlace : T{};
            case 6:
                return !std::memcmp(string.begin(), "castle", 6) ? T::kCastle : T{};
            case 7:
                switch (string[2]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "amenity", 7) ? T::kAmenity
                            : T{};
                    case 'r':
                        return !std::memcmp(string.begin(), "parking", 7) ? T::kParking
                            : T{};
                    default:
                        return !std::memcmp(string.begin(), "glacier", 7) ? T::kGlacier
                            : T{}; //case 'a'
                }
            default:
                switch (string[2]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "viewpoint", 8) ? T::kViewpoint
                            : T{};
                    case 'n':
                        return string == "funicularStation" ? T::kFunicular
                            : T{};
                    case 't':
                        return !std::memcmp(string.begin(), "waterfall", 9) ? T::kWaterfall
                            : T{};
                    default:
                        return !std::memcmp(string.begin(), "alpineHut", 8) ? T::kMountainHut
                            : T{};
                }
        }
    }
};

using T = DVGGeoDatabaseEntry::Type;
static auto EntryTypeString = generateTestStrings<EntryTypeMapping>();

struct FrozenBySizeAndThird {
    static constexpr
    frozen::unordered_map<frozen::string, T, 12, ThirdAndSizeElsa> map = {
        {"alpineHut", T::kMountainHut},
        {"amenity", T::kAmenity},
        {"castle", T::kCastle},
        {"viewpoint", T::kViewpoint},
        {"parking", T::kParking},
        {"funicularStation", T::kFunicular},
        {"waterfall", T::kWaterfall},
        {"place", T::kPlace},
        {"glacier", T::kGlacier},
        {"lake", T::kLake},
        {"pass", T::kPass},
        {"peak", T::kPeak}
    };

    static inline T decode(const std::string_view &view) {
        auto it = map.find(frozen::string(&view.front(), view.size()));
        if (it == map.end()) {
            return T{};
        }
        return it->second;
    }
};

struct FrozenDefault {
    static constexpr
    frozen::unordered_map<frozen::string, T, 12> map = {
        {"alpineHut", T::kMountainHut},
        {"amenity", T::kAmenity},
        {"castle", T::kCastle},
        {"viewpoint", T::kViewpoint},
        {"parking", T::kParking},
        {"funicularStation", T::kFunicular},
        {"waterfall", T::kWaterfall},
        {"place", T::kPlace},
        {"glacier", T::kGlacier},
        {"lake", T::kLake},
        {"pass", T::kPass},
        {"peak", T::kPeak}
    };

    static inline T decode(const std::string_view &view) {
        auto it = map.find(frozen::string(&view.front(), view.size()));
        if (it == map.end()) {
            return T{};
        }
        return it->second;
    }
};

struct SwitchForthWOVerification {
    static inline T decode(const std::string_view &string) {
        if (string.size() >= 4) {
            switch (string[3]) {
                case 'c': return string[0] == 'p' ? T::kPlace : T::kGlacier;
                case 'e': return string[0] == 'w' ? T::kWaterfall : T::kLake;
                case 'i': return string[0] == 'a' ? T::kMountainHut : T::kFunicular;
                case 'k': return string[1] == 'e' ? T::kPeak : T::kParking;
                case 'n': return T::kAmenity;
                case 's': return T::kPass;
                case 't': return T::kCastle;
                default: return T::kViewpoint; // i.e. case 'w'
            }
        }
        return {};
    }
};

struct SwitchSecondWOVerification {
    static inline T decode(const std::string_view &string) {
        if (string.size() >= 4) {
            switch (string[1]) {
                case 'a':
                    switch (string[2]) {
                        case 'r': return T::kParking;
                        case 's': return string[0] == 'c' ? T::kCastle : T::kPass;
                        case 't': return T::kWaterfall;
                        default: return T::kLake; // case 'k'
                    }
                case 'e': return T::kPeak;
                case 'i': return T::kViewpoint;
                case 'l':
                    switch (string[0]) {
                        case 'a': return T::kMountainHut;
                        case 'p': return T::kPlace;
                        default: return T::kGlacier; //case 'g'
                    }
                case 'm': return T::kAmenity;
                default: return T::kFunicular; // case 'u'
            }
        }
        return {};
    }
};

struct SwitchForth {
    static inline T decode(const std::string_view &string) {
        if (string.size() >= 4) {
            switch (string[3]) {
                case 'c':
                    if (string[0] == 'p') {
                        return string == std::string_view{"place"} ? T::kPlace : T::kUnknown;
                    }
                    return string == std::string_view{"glacier"} ? T::kGlacier : T::kUnknown;
                case 'e':
                    if (string[0] == 'w') {
                        return string == std::string_view{"waterfall"} ? T::kWaterfall
                            : T::kUnknown;
                    }
                    return string == std::string_view{"lake"} ? T::kLake : T::kUnknown;
                case 'i':
                    if (string[0] == 'a') {
                        return string == std::string_view{"alpineHut"} ? T::kMountainHut
                            : T::kUnknown;
                    }
                    return string == std::string_view{"funicularStation"} ? T::kFunicular
                        : T::kUnknown;
                case 'k':
                    if (string[1] == 'e') {
                        return string == std::string_view{"peak"} ? T::kPeak : T::kUnknown;
                    }
                    return string == std::string_view{"parking"} ? T::kParking : T::kUnknown;
                case 'n':
                    return string == std::string_view{"amenity"} ? T::kAmenity : T::kUnknown;
                case 's':
                    return string == std::string_view{"pass"} ? T::kPass : T::kUnknown;
                case 't':
                    return string == std::string_view{"castle"} ? T::kCastle : T::kUnknown;
                default:
                    return string == std::string_view{"viewpoint"} ? T::kViewpoint
                        : T::kUnknown; // i.e. case 'w'
            }
        }
        return {};
    }
};

struct SwitchSizeWOVerification {
    static inline T decode(const std::string_view &string) {
        if (string.size() < 4) {
            return DVGGeoDatabaseEntry::Type::kUnknown;
        }
        switch (string.size()) {
            case 4:
                switch (string[2]) {
                    case 'k': return T::kLake;
                    case 's': return T::kPass;
                    default: return T::kPeak; //case 'a'
                }
            case 5: return T::kPlace;
            case 6: return T::kCastle;
            case 7:
                switch (string[2]) {
                    case 'e': return T::kAmenity;
                    case 'r': return T::kParking;
                    default: return T::kGlacier; //case 'a'
                }
            default: // case 8+
                switch (string[2]) {
                    case 'e': return T::kViewpoint;
                    case 'n': return T::kFunicular;
                    case 't': return T::kWaterfall;
                    default: return T::kMountainHut; // case 'p'
                }
        }
    }
};

struct SwitchSize {
    static inline T decode(const std::string_view &string) {
        if (string.size() < 4) {
            return {};
        }
        switch (string.size()) {
            case 4:
                switch (string[2]) {
                    case 'k': return string == "lake" ? T::kLake : T::kUnknown;
                    case 's': return string == "pass" ? T::kPass : T::kUnknown;
                    default: return string == "peak" ? T::kPeak : T::kUnknown; //case 'a'
                }
            case 5: return string == "place" ? T::kPlace : T::kUnknown;
            case 6: return string == "castle" ? T::kCastle : T::kUnknown;
            case 7:
                switch (string[2]) {
                    case 'e': return string == "amenity" ? T::kAmenity : T{};
                    case 'r': return string == "parking" ? T::kParking : T{};
                    default: return string == "glacier" ? T::kGlacier : T{};
                }
            default:
                switch (string[2]) {
                    case 'e': return string == "viewpoint" ? T::kViewpoint : T{};
                    case 'n': return string == "funicularStation" ? T::kFunicular : T{};
                    case 't': return string == "waterfall" ? T::kWaterfall : T{};
                    default: return string == "alpineHut" ? T::kMountainHut : T{};
                }
        }
    }
};

struct SwitchSizeMemcmp {
    static inline T decode(const std::string_view &string) {
        switch (string.size()) {
            case 4:
                switch (string[2]) {
                    case 'k':
                        return !std::memcmp(string.begin(), "lake", 4) ? T::kLake : T::kUnknown;
                    case 's':
                        return !std::memcmp(string.begin(), "pass", 4) ? T::kPass : T::kUnknown;
                    default:
                        return !std::memcmp(string.begin(), "peak", 4) ? T::kPeak
                            : T::kUnknown; //case 'a'
                }
            case 5:
                return !std::memcmp(string.begin(), "place", 5) ? T::kPlace : T::kUnknown;
            case 6:
                return !std::memcmp(string.begin(), "castle", 6) ? T::kCastle : T::kUnknown;
            case 7:
                switch (string[2]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "amenity", 7) ? T::kAmenity
                            : T::kUnknown;
                    case 'r':
                        return !std::memcmp(string.begin(), "parking", 7) ? T::kParking
                            : T::kUnknown;
                    default:
                        return !std::memcmp(string.begin(), "glacier", 7) ? T::kGlacier
                            : T::kUnknown; //case 'a'
                }
            default:
                switch (string[2]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "viewpoint", 8) ? T::kViewpoint
                            : T::kUnknown;
                    case 'n':
                        return string == "funicularStation" ? T::kFunicular
                            : T::kUnknown;
                    case 't':
                        return !std::memcmp(string.begin(), "waterfall", 9) ? T::kWaterfall
                            : T::kUnknown;
                    default:
                        return !std::memcmp(string.begin(), "alpineHut", 8) ? T::kMountainHut
                            : T::kUnknown;
                }
        }
    }
};

#define INT_REP_CASE(x) case x: return T::x;

struct IntRep {
    static inline uint64_t strToInt(const std::string_view &string) {
        uint64_t local{};
        std::memcpy(&local, string.begin(), std::min(string.size(), static_cast<unsigned long>(8u)));
        return local;
    }

    static inline T decode(const std::string_view &string) { //
        if (string.size() < 4) {
            return {};
        }

        static const uint64_t kPeak = 1801545072;
        static const uint64_t kPass = 1936941424;
        static const uint64_t kLake = 1701536108;
        static const uint64_t kGlacier = 32199650658315367;
        static const uint64_t kPlace = 435459026032;
        static const uint64_t kWaterfall = 7809635870351581559;
        static const uint64_t kFunicular = 7020114988994819430;
        static const uint64_t kParking = 29113321654477168;
        static const uint64_t kViewpoint = 7956012745556846966;
        static const uint64_t kCastle = 111516484591971;
        static const uint64_t kAmenity = 34186468354518369;
        static const uint64_t kMountainHut = 8451116225650519137;

        if (string.size() > 8) {
            uint64_t intRep{};
            std::memcpy(&intRep, string.begin(), 8);
            switch (intRep) {
                INT_REP_CASE(kWaterfall)
                INT_REP_CASE(kFunicular)
                INT_REP_CASE(kViewpoint)
                INT_REP_CASE(kMountainHut)
                default: return {};
            }
        }
        uint64_t intRep{};
        std::memcpy(&intRep, string.begin(), string.size());

        switch (intRep) {
            INT_REP_CASE(kPeak)
            INT_REP_CASE(kPass)
            INT_REP_CASE(kLake)
            INT_REP_CASE(kGlacier)
            INT_REP_CASE(kPlace)
            INT_REP_CASE(kParking)
            INT_REP_CASE(kCastle)
            INT_REP_CASE(kAmenity)
            default:
                return T::kUnknown;
        }
    }
};

struct EntryType {
    using Mapping = EntryTypeMapping;

    static inline std::vector<std::string_view> &views() {
        return EntryTypeString.views;
    }
};

#define ENTRY_TYPE_BENCHMARK(Decoder) VERIFIED_BENCHMARK(Decoder, EntryType)

ENTRY_TYPE_BENCHMARK(SwitchDecoder<EntryTypeMapping>);
ENTRY_TYPE_BENCHMARK(DefaultDecoder<EntryTypeMapping>);
ENTRY_TYPE_BENCHMARK(FrozenBySizeAndThird);
ENTRY_TYPE_BENCHMARK(FrozenDefault);
ENTRY_TYPE_BENCHMARK(SwitchForth);
ENTRY_TYPE_BENCHMARK(SwitchSize);
ENTRY_TYPE_BENCHMARK(SwitchSizeMemcmp);
ENTRY_TYPE_BENCHMARK(IntRep);
ENTRY_TYPE_BENCHMARK(SwitchSizeWOVerification);
ENTRY_TYPE_BENCHMARK(SwitchForthWOVerification);
ENTRY_TYPE_BENCHMARK(SwitchSecondWOVerification);

} // namespace


#endif //BENCH_LAB_GEO_ENTRY_H
