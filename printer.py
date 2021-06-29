import random
from datetime import datetime
random.seed(datetime.now())

NO_HASH = -2
SIZE_INDEX = -1

def maxl(strings):
    return max([len(x) for x in strings])

def minl(strings):
    return min([len(x) for x in strings])

def differentiate(strings, hash_fun):
    d = dict()
    for s in strings:
        h = hash_fun(s)
        if h == NO_HASH:
            return dict()
        if h not in d:
            d[h] = []
        d[h].append(s)
    return d

def get_rule(rule_index):
    if rule_index == SIZE_INDEX:
        return lambda x: len(x)
    return lambda x: x[rule_index] if rule_index < len(x) else NO_HASH

def combine_two_hashes(first_hash, second_hash):
    return lambda x: str(first_hash(x)) + str(second_hash(x)) if NO_HASH not in (first_hash, second_hash(x)) else NO_HASH

def combine_hashes(hashes):
    if len(hashes) == 1:
        return hashes[0]
    return combine_hashes(hashes[:-2] + [combine_two_hashes(hashes[-2], hashes[-1])])

def differentiate_by_rules(strings, rules):
    hash_fun = combine_hashes([get_rule(x) for x in rules])
    return differentiate(strings, hash_fun)

def next_rule(rule, min_length):
    rule_index_to_increment = -1
    while rule[rule_index_to_increment] == rule_index_to_increment + min_length:
        if rule_index_to_increment == -len(rule):
            return None
        rule_index_to_increment -= 1
    rule[rule_index_to_increment] += 1
    for i in range(len(rule) + rule_index_to_increment + 1, len(rule)):
        rule[i] = rule[i - 1] + 1
    return rule

def find_differentiating_rule(strings):
    m = minl(strings)
    best_rule = None
    best_rule_score = 0
    for i in range(1, m + 2):
        rule = [x - 1 for x in range(i)]
        while rule is not None:
            diff = differentiate_by_rules(strings, rule)
            if not best_rule or best_rule_score < len(diff):
                best_rule = rule
                best_rule_score = len(diff)
                if best_rule_score == len(strings):
                    return best_rule
            rule = next_rule(rule, m)

    return best_rule

class Printer:
    def __init__(self):
        self._ind = 0
        self.text = ""

    def indent(self):
        self._ind += 4

    def deindent(self):
        self._ind -= 4
        assert(self._ind >= 0)

    def get_indent_str(self):
        return " " * self._ind

    def __call__(self, s=""):
        assert(self._ind >= 0)
        if s == "":
            self.text += "\n"
        else:
            self.text += " " * self._ind + s + "\n"

class EnumField:
    def __init__(self, name, string):
        self.name = name
        self.string = string
        self.size_is_known = False

    def memcmp_check(self):
        return '!std::memcmp(string.begin(), "' + self.string + '", ' + str(len(self.string)) + ")"

    def eq_check(self):
        return 'string == "' + self.string + '"'

    def check(self):
        return self.memcmp_check() if self.size_is_known else self.eq_check()

    def return_string_or_empty(self):
        return "return " + self.check() + " ? " + self.name + " : E{};"

    def check_return(self):
        return "if (" + self.check() + ") return " + self.name + ";"

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

    p("switch (string" + (".size()" if by_size else "[" + str(rules[0]) + "]") + ") {")
    p.indent()

    for case_fields in switch_arr:
        case_value = case_fields[0]
        is_last = case_value == switch_arr[-1][0]
        assert(case_value != NO_HASH)
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
    p("static inline E decode(std::string_view string) {")
    p.indent()
    min_length = min([len(x.string) for x in fields])
    if rule[0] != SIZE_INDEX:
        p("if (string.size() < " + str(min_length) + ") { return E{}; }")
    generate_switches(p, rule, fields)
    p("return E{};")
    p.deindent()
    p("}")
