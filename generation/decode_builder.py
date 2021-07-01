import random
from datetime import datetime
from generation.printer import Printer
from generation.rules import *
from generation.enum_field import EnumField
from generation.parser import *

random.seed(datetime.now())

def generate_switches(p: Printer, rules, fields):
    switch_map = dict()
    rule = get_rule(rules[0])
    by_size = rules[0] == SIZE_INDEX
    for field in fields:
        if by_size:
            field.size_is_known = True
        i = rule(field.string)
        if i not in switch_map:
            switch_map[i] = []
        switch_map[i].append(field)

    switch_arr = [(x, switch_map[x]) for x in switch_map]
    switch_arr.sort(key=lambda x: x[0])
    if len(switch_arr) == 1:
        for field in switch_arr[0][1]:
            if by_size:
                field.size_is_known = False
            p(field.check_return())
        p("return E{};")
        return

    # if len(swi)

    p("switch (string" + (".size()" if by_size else "[" + str(rules[0]) + "]") + ") {")
    p.indent()

    for case_fields in switch_arr:
        case_value = case_fields[0]
        is_last = case_value == switch_arr[-1][0]
        assert(case_value != NO_HASH)

        if not is_last:
            for field in case_fields[1]:
                field.known_fields.append(rules[0])
                # field.size_is_known = False

        case_prefix = "default" if is_last \
            else "case " + (str(case_value) if by_size else "'" + case_value + "'")
        p(case_prefix + ":")
        if is_last and by_size:
            for field in case_fields[1]:
                field.size_is_known = False
        p.indent()
        if len(case_fields[1]) == 1:
            p(case_fields[1][0].return_string_or_empty())
        elif len(rules) == 1:
            for field in case_fields[1]:
                p(field.check_return())
            p("return E{};")
        else:
            generate_switches(p, rules[1:], case_fields[1])
        p.deindent()

    p.deindent()
    p("}")

def generate_decode(p: Printer, fields):
    strings = [x.string for x in fields]
    rule = find_differentiating_rule(strings)
    p("static inline __attribute__((always_inline))  E decode(const std::string_view &string) {")
    p.indent()
    min_length = min([len(x.string) for x in fields])
    if rule[0] != SIZE_INDEX:
        p("if (string.size() < " + str(min_length) + ") { return E{}; }")
    generate_switches(p, rule, fields)
    p("return E{};")
    p.deindent()
    p("}")

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
