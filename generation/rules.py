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
    m = min([len(x) for x in strings])
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
