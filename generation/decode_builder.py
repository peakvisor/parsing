import random
from datetime import datetime
from generation.printer import Printer
from generation.rules import *
from generation.enum_field import EnumField
from generation.parser import *
from generation import decision_tree

random.seed(datetime.now())

def generate_switches_from_tree(p: Printer, tree: decision_tree.Node, strings_to_names):
    switch_arg_string = ""
    if tree.rule == SIZE_INDEX:
        switch_arg_string = "string.size()"
    else:
        if tree.length_check:
            switch_arg_string += "string.size() < " + str(tree.checked_length) + " ? '\\0' : "
        switch_arg_string += "string[" + str(tree.rule) + "]"
    p("switch(" + switch_arg_string + ") {")
    p.indent()
    value_kids = sorted([(key, value) for key, value in tree.kids.items()], key=lambda x: x[0])
    had_default = False
    for value, kid in value_kids:
        if value == value_kids[-1][0] and tree.rule != SIZE_INDEX:
            had_default = True
            p("default: ")
        else:
            value_string = "'" + str(value) + "'" if tree.rule != SIZE_INDEX else str(value)
            p("case " + value_string + ":")

        p.indent()
        if kid.is_leaf():
            name = strings_to_names[kid.strings[0]]
            field = EnumField(name, kid.strings[0])
            field.known_fields = kid.prev_rules
            p(field.return_string_or_empty())
        else:
            generate_switches_from_tree(p, kid, strings_to_names)
        p.deindent()
    if not had_default:
        p("default: return E{};")
    p.deindent()
    p("}")

def generate_switches_via_tree(p: Printer, fields):
    strings_to_names = {f.string: f.name for f in fields}
    strings = {f.string for f in fields}
    tree = decision_tree.Node(strings, set(), 0)
    generate_switches_from_tree(p, tree, strings_to_names)

def generate_switches(p: Printer, rules, fields):
    switch_map = dict()
    rule = get_rule_hash(rules[0])
    by_size = rules[0] == SIZE_INDEX
    for field in fields:
        i = rule(field.string)
        if i not in switch_map:
            switch_map[i] = []
        switch_map[i].append(field)

    switch_arr = [(x, switch_map[x]) for x in switch_map]
    switch_arr.sort(key=lambda x: x[0])
    if len(switch_arr) == 1:
        for field in switch_arr[0][1]:
            p(field.check_return())
        p("return E{};")
        return

    p("switch (string" + (".size()" if by_size else "[" + str(rules[0]) + "]") + ") {")
    p.indent()

    for case_fields in switch_arr:
        case_value = case_fields[0]
        is_last = case_value == switch_arr[-1][0]
        assert(case_value != NO_HASH)

        if not is_last:
            for field in case_fields[1]:
                field.known_fields.append(rules[0])

        case_prefix = "default" if is_last \
            else "case " + (str(case_value) if by_size else "'" + case_value + "'")
        p(case_prefix + ":")
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
    p("static inline __attribute__((always_inline)) E decode(const std::string_view &string) {")
    p.indent()
    generate_switches_via_tree(p, fields)
    p.deindent()
    p("}")

def generate_memcmp_value_for_key_decode(p: Printer):
    p("static inline __attribute__((always_inline)) E decode(const std::string_view &string) {")
    p.indent()
    p("return DVGKeyPair<E>::valueForKeyMemcmp(StringView{string}, values, E{});")
    p.deindent()
    p("}")

def add_decode(src, benchmark_wrapper=False, reps=0):
    mappings = find_mappings(src)
    if benchmark_wrapper:
        mapping_names = [get_mapping_name(src[m.start]) for m in mappings]
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

    if benchmark_wrapper:
        src = add_benchmark_wrapper(src, mapping_names, reps)

    return src

def add_benchmark_wrapper(src, mapping_names, reps):
    prefix = [
        "namespace generated_mappings {\n",
        "\n"
    ]
    if reps > 0:
        prefix += [
            "static constexpr size_t kRepetitions = " + str(reps) + ";\n"
            "\n"
        ]
    suffix = ["\n"]
    suffix += ["TEST_MAPPING(" + m + ")\n" for m in mapping_names]
    suffix.append("\n")
    suffix.append("} // namespace\n")

    return prefix + src + suffix
