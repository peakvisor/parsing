#ifndef BENCH_LAB_TESTING_H
#define BENCH_LAB_TESTING_H

#include "read_strings.h"
#include "dvg.h"

#include <frozen/unordered_map.h>
#include <frozen/string.h>

static constexpr size_t kRepetitions = 5;
static constexpr bool kGenerateRandom = true;

struct FirstThreeElsa {
    constexpr std::size_t operator()(frozen::string value) const {
        std::size_t d = 5381;
        for (std::size_t i = 0; i < 3; ++i)
            d = d * 33 + static_cast<size_t>(value[i]);
        return d;
    }

    constexpr std::size_t operator()(frozen::string value, std::size_t seed) const {
        std::size_t d =  (0x811c9dc5 ^ seed) * static_cast<size_t>(0x01000193);
        for (std::size_t i = 0; i < 3; ++i)
            d = (d ^ static_cast<size_t>(value[i])) * static_cast<size_t>(0x01000193);
        return d >> 8 ;
    }
};

struct ThirdAndSizeElsa {
    constexpr std::size_t operator()(frozen::string value) const {
        std::size_t d = 5381;

        d = d * 33 + static_cast<size_t>(value[2]);
        d = d * 33 + static_cast<size_t>(value.size());
        return d;
    }

    constexpr std::size_t operator()(frozen::string value, std::size_t seed) const {
        std::size_t d =  (0x811c9dc5 ^ seed) * static_cast<size_t>(0x01000193);
        d = (d ^ static_cast<size_t>(value[2])) * static_cast<size_t>(0x01000193);
        d = (d ^ static_cast<size_t>(value.size())) * static_cast<size_t>(0x01000193);
        return d >> 8 ;
    }
};

struct FirstElsa {
    constexpr std::size_t operator()(frozen::string value) const {
        return 5381 * 33 + static_cast<size_t>(value[0]);
    }

    constexpr std::size_t operator()(frozen::string value, std::size_t seed) const {
        std::size_t d =  (0x811c9dc5 ^ seed) * static_cast<size_t>(0x01000193);
        d = (d ^ static_cast<size_t>(value[0])) * static_cast<size_t>(0x01000193);
        return d >> 8 ;
    }
};

template<typename Decoder, typename EnumTraits>
void Test(benchmark::State &state) {
    for (auto _ : state) {
        for (const auto &v : EnumTraits::views()) {
            auto type = Decoder::decode(v);
            benchmark::DoNotOptimize(type);
        }
    }
    state.SetItemsProcessed(state.iterations() * EnumTraits::views().size());
}

bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

#define VERIFIED_BENCHMARK(Decoder, EnumTraits)\
    static bool CONCAT(staticCheck, __LINE__) = [] { \
        if (hasEnding(#Decoder, "WOVerification")) { return true; } \
        std::cout << "checking " << __LINE__ << " " << #Decoder << " with " << #EnumTraits;\
        for (const auto &v : EnumTraits::views()) {\
            if (!(Decoder::decode(v) == DefaultDecoder<EnumTraits::Mapping>::decode(v))) {\
                std::cout << "\n" <<  v << " gives diff" << std::endl;\
                assert(false);\
            }\
        }\
        std::cout << " checked\n";                                         \
        return true;\
     }();\
BENCHMARK_TEMPLATE2(Test, Decoder, EnumTraits)->Repetitions(kRepetitions)

#define RESOLVED_MACRO_BENCHMARK(Test, Decoder, EnumTraits) BENCHMARK_TEMPLATE2(Test, Decoder, EnumTraits)->Repetitions(kRepetitions)

#define TEST_MAPPING(NAME) \
static auto CONCAT(NAME, Strings) = generateTestStrings<CONCAT(NAME, Mapping)>();\
struct CONCAT(NAME, Traits) {\
    using Mapping = CONCAT(NAME, Mapping);\
    static inline std::vector<std::string_view> &views() {\
        return CONCAT(NAME, Strings).views;\
    }\
};\
VERIFIED_BENCHMARK(SwitchDecoder<CONCAT(NAME, Mapping)>, CONCAT(NAME, Traits));    \
RESOLVED_MACRO_BENCHMARK(Test, DefaultDecoder<CONCAT(NAME, Mapping)>, CONCAT(NAME, Traits));

#endif //BENCH_LAB_TESTING_H
