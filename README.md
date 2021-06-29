# parsing

Given an input C++ file, such as enumExamples.hpp, hash.py writes modified file to output.h and to path provided by the first script argument. It replaces (or adds) decode static method to all ...Mapping structs with static constexpr DVGKeyPair<E> values[] variable.
Input file parsing is not as smart as full-fledged C++ compiler so it is better to follow the style guide suggested by enumExamples.hpp.
