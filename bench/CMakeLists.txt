cmake_minimum_required(VERSION 3.1)
project(fast_enum_bench)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_CXX_STANDARD 17)

add_executable(fast_enum_bench main.cpp)
add_subdirectory(google_benchmark)

target_include_directories(fast_enum_bench PUBLIC /Users/ka/src_local/frozen/include)
target_link_libraries(fast_enum_bench benchmark)
