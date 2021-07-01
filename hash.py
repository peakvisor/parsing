from generation.parser import *
from generation.decode_builder import *

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
    f = open("data/input_header.h")
    src = f.readlines()
    f.close()
    src = add_decode(src)
    o = open("bench/mappings.h", "w")
    for line in src:
        o.write(line)
    o.close()

if __name__ == "__main__":
    main()
