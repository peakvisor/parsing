# installation

git clone https://github.com/peakvisor/parsing.git

cd parsing

git submodule update --init

git clone https://github.com/google/googletest.git bench/google_benchmark/googletest

# generation

Basic usage:

<code> cat input_header.h | python3 main.py > output_header.h </code>

Generation replaces (or adds) decode static method to all ...Mapping structs with static constexpr DVGKeyPair<E> values[] variable.
Input file parsing is not as smart as C++ compiler, so it is better to follow the style guide suggested by enumExamples.hpp. 

To generate headers for benchmarking, use from the root dir:

<code>python3 main.py generate repetitions</code>

where repetitions is optional and sets amount of times each benchmark is run.

To save mappings.h and generation python files for future reference, use

<code>python3 main.py save</code>

# benchmarking

bench directory contains CMake project which builds the banchmark.  Its fast_enum_benchmark target launches the benchmark. To control what is benchmarked, comment out headers in bench/main.cpp. To control amount of repetitions, set kRepetitions in bench/bench_common.h.
Keep in mind that it was noticed that launching benchmark with other big processes running leads to very volatile results.

You can also launch benchmarking (including all the building) via python:

<code>python3 bench_compare compiler</code>

where compiler is a path to your C++ compiler of choice. C++ build files are created in a build folder, and in out.txt there you will find benchmarking output. In stdout there will be benchmarking comparison useful to compare results of two versions of generations. For that, include only those two mappings headers in main.cpp:

<code>#include "prev_mappings.h"</code>

<code>#include "mappings.h" </code>