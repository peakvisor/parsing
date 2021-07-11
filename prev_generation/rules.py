from copy import deepcopy
from collections import defaultdict

NO_HASH = -2
SIZE_INDEX = -1

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

def get_rule_hash(rule_index):
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
    hash_fun = combine_hashes([get_rule_hash(x) for x in rules])
    return differentiate(strings, hash_fun)

def differentiate_by_one_rule(strings, rule):
    return differentiate(strings, get_rule_hash(rule))

def get_next_rule(rule, min_length):
    rule_index_to_increment = -1
    while rule[rule_index_to_increment] == rule_index_to_increment + min_length:
        if rule_index_to_increment == -len(rule):
            return None
        rule_index_to_increment -= 1
    rule[rule_index_to_increment] += 1
    for i in range(len(rule) + rule_index_to_increment + 1, len(rule)):
        rule[i] = rule[i - 1] + 1
    return rule

def switch_complexity(strings, rule):
    buckets = [strings]
    complexity = 0
    rule_number = 0
    for r in rule:
        rule_number += 1
        rule_hash = get_rule_hash(r)
        next_buckets = []
        for b in buckets:
            key_to_bucket = defaultdict(list)
            for s in b:
                key_to_bucket[rule_hash(s)].append(s)


def find_differentiating_rule(strings):
    m = min([len(x) for x in strings])
    best_rule_image_size = 0

    best_rules = []

    for i in range(1, m + 2):
        rule = [x - 1 for x in range(i)]
        while rule is not None:
            diff = differentiate_by_rules(strings, rule)
            image_size = len(diff)
            if image_size > best_rule_image_size:
                best_rules = [rule]
                best_rule_image_size = image_size
            elif image_size == best_rule_image_size:
                best_rules.append(rule)
            # if not best_rule or best_rule_image_size < len(diff):
            #     best_rules.append(rule)
            #     best_rule = rule
            #     best_rule_image_size = len(diff)
            #     if best_rule_image_size == len(strings):
            #         return best_rule
            rule = get_next_rule(rule, m)

    if best_rule_image_size < len(strings):
        print("// strings cant be fully differentiated: " + str(strings))

    min_complexity = len(strings) * (m + 1)
    min_complexity_rule = None

    for rule in best_rules:
        complexity = switch_complexity(strings, rule)
        if complexity < min_complexity:
            min_complexity = complexity
            min_complexity_rule = rule


    return min_complexity_rule
