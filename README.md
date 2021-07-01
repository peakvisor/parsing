# installation

git clone https://github.com/peakvisor/parsing.git

cd parsing

git submodule update --init

git clone https://github.com/google/googletest.git bench/google_benchmark/googletest

# generation

python3 hash.py uses data/input_header.h to generate bench/mappings.h which is then can be used in benchmarking. Generation replaces (or adds) decode static method to all ...Mapping structs with static constexpr DVGKeyPair<E> values[] variable.
Input file parsing is not as smart as full-fledged C++ compiler so it is better to follow the style guide suggested by enumExamples.hpp.

# benchmarking

bench directory contains CMake project which builds the banchmark.  Its fast_enum_benchmark target launches the benchmark. To control what is benchmarked, comment out headers in bench/main.cpp. To control amount of repetitions, set kRepetitions in bench/bench_common.h.
Keep in mind that it was noticed that launching benchmark with other big processes running leads to very volatile results.
