from parser import *
from printer import *
from sys import argv

def add_decode(src):
    mappings = find_mappings(src)
    mappings.reverse()

    for m in mappings:
        values = m.find_sub_block("DVGKeyPair<E> values")
        enum = parse_values(values.insides_to_str())
        fields = [EnumField(x[0], x[1]) for x in enum]
        p = Printer()
        p.indent()
        generate_decode(p, fields)
        decode_space = m.find_sub_block("decode(std::string_view string)")

        if src[decode_space.start - 1].strip() == "":
            decode = [p.text]
        else:
            decode = [p.get_indent_str() + "\n", p.text]

        src = src[:decode_space.start] + decode + src[decode_space.end:]
    return src

def main():
    f = open("enumExamples.hpp")
    src = f.readlines()
    f.close()
    src = add_decode(src)
    f = open(argv[1], "w")
    o = open("output.h", "w")
    f.write("#ifndef MAPPINGS_H\n")
    f.write("#define MAPPINGS_H\n\n")
    for line in src:
        f.write(line)
        o.write(line)
    f.write("\n#endif // MAPPINGS_H\n")
    f.close()

if __name__ == "__main__":
    main()
