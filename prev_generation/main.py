from generation.decode_builder import add_decode
from generation import parse_bench_results

from generation.decision_tree import *
from generation.printer import Printer

import sys
import os
from subprocess import run
import shutil

def generate(input_register, output_register, benchmark_wrapper=False, reps=0):
    src = input_register.readlines()
    src = add_decode(src, benchmark_wrapper, reps)
    for line in src:
        output_register.write(line)

def bench(cxx_compiler):
    os.chdir("build")
    run(["cmake", "-S", "../bench", "-DCMAKE_BUILD_TYPE=Release",
         "-DCMAKE_CXX_COMPILER="+cxx_compiler])
    run(["make", "fast_enum_bench"])
    r = run("./fast_enum_bench", capture_output=True)
    out = r.stdout.decode("utf8")
    open("out.txt", "w").write(out)
    parse_bench_results.report([line for line in out.split("\n") if "median" in line])

def save():
    src = open("bench/mappings.h").readlines()
    src[0] = src[0].replace("generated_mappings", "prev_generated_mappings")
    with open("bench/prev_mappings.h", "w") as prev_mappings:
        for line in src:
            prev_mappings.write(line)
    shutil.copytree("generation", "prev_generation", dirs_exist_ok=True)

def printTree(tree, p: Printer):
    p("<" + str(tree.rule) + ">, c = " + str(tree.complexity()) + ":")
    p.indent()
    for value, kid in tree.kids.items():
        p(str(value) + ":")
        p.indent()
        if kid.is_leaf():
            p(kid.strings[0])
        else:
            printTree(kid, p)
        p.deindent()
    p.deindent()

def printTreeForTest(self):
    strings = ["Ultra",
               "Top10",
               "Top50",
               "14er",
               "13er",
               "Volcano",
               "Glacier",
               "Munro",
               "Furth",
               "TGO40",
               "JA100",
               "TW100",
               "AHP46"]
    tree = Node(strings, set())
    p = Printer()
    printTree(tree, p)
    print(p.text)

def main():
    if len(sys.argv) > 1:
        if sys.argv[1] == "generate":
            reps = int(sys.argv[2]) if len(sys.argv) > 2 else 5
            generate(open("data/input_header.h"), open("bench/mappings.h", "w"), True, reps=reps)
        elif sys.argv[1] == "save":
            save()
        elif sys.argv[1] == "bench":
            bench(sys.argv[2])
    else:
        generate(sys.stdin, sys.stdout)
