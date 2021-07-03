from generation.decode_builder import add_decode
from generation import parse_bench_results
import sys
import os
from subprocess import run

def generate(input_register, output_register, benchmark_wrapper=False, reps=0):
    src = input_register.readlines()
    src = add_decode(src, benchmark_wrapper, reps)
    for line in src:
        output_register.write(line)

def launch(cxx_compiler):
    print("__file__")
    os.chdir("build")
    run(["cmake", "-S", "../bench", "-DCMAKE_BUILD_TYPE=Release",
         "-DCMAKE_CXX_COMPILER="+cxx_compiler])
    run(["make", "fast_enum_bench"])
    r = run("./fast_enum_bench", capture_output=True)
    out = r.stdout.decode("utf8")
    parse_bench_results.report([line for line in out.split("\n") if "median" in line])

def main():
    if len(sys.argv) > 1:
        if sys.argv[1] == "generate":
            reps = int(sys.argv[2]) if len(sys.argv) > 2 else 5
            generate(open("data/input_header.h"), open("bench/mappings.h", "w"), True, reps=reps)
        elif sys.argv[1] == "save":
            src = open("bench/mappings.h").readlines()
            src[0] = src[0].replace("generated_mappings", "prev_generated_mappings")
            with open("bench/prev_mappings.h", "w") as prev_mappings:
                for line in src:
                    prev_mappings.write(line)
        elif sys.argv[1] == "bench":
            launch(sys.argv[2])
    else:
        generate(sys.stdin, sys.stdout)

if __name__ == "__main__":
    main()
