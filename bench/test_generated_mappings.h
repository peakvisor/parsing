#ifndef BENCH_LAB_TEST_GENERATED_MAPPINGS_H
#define BENCH_LAB_TEST_GENERATED_MAPPINGS_H

#include "dvg.h"
#include "mappings.h"
#include "read_strings.h"
#include "bench_common.h"

#include <vector>
#include <string>
#include <string_view>

namespace generated_mappings_test {

#define TEST_MAPPING(NAME) \
static auto CONCAT(NAME, Strings) = generateTestStrings<CONCAT(NAME, Mapping)>();\
struct CONCAT(NAME, Traits) {\
    using Mapping = CONCAT(NAME, Mapping);\
    static inline std::vector<std::string_view> &views() {\
        return CONCAT(NAME, Strings).views;\
    }\
};\
VERIFIED_BENCHMARK(ProdDecoder<CONCAT(NAME, Mapping)>, CONCAT(NAME, Traits));    \
RESOLVED_MACRO_BENCHMARK(Test, DefaultDecoder<CONCAT(NAME, Mapping)>, CONCAT(NAME, Traits)) ->Repetitions(kRepetitions);

TEST_MAPPING(Type)
TEST_MAPPING(Color)
TEST_MAPPING(Tags)
TEST_MAPPING(FunicularType)
TEST_MAPPING(EntryType)
TEST_MAPPING(Legacy_EntryType)
TEST_MAPPING(PeakCategory)
TEST_MAPPING(ResourceFormat)
TEST_MAPPING(ResourceType)

}

#endif //BENCH_LAB_TEST_GENERATED_MAPPINGS_H
