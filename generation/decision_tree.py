from generation.rules import *
from copy import deepcopy

LENGTH_SPECIFIED = -5

class Node:
    def __init__(self, strings, prev_rules: set, checked_length):
        assert strings
        self.strings = strings
        self.kids: list(Node) = dict()
        self.rule = None
        self.prev_rules = sorted(list(prev_rules))
        self.checked_length = checked_length
        self.length_check = False

        if len(self.strings) == 1:
            return

        minl = min([len(s) for s in strings])
        min_complexity = len(strings) * (minl + 1)
        max_rule = minl if prev_rules else 0

        for rule in range(-1, max_rule):
            if rule not in prev_rules:
                next_rules = deepcopy(prev_rules)
                next_rules.add(rule)
                assert len(strings) > 1
                diff = differentiate_by_one_rule(strings, rule)
                if len(diff) == 1:
                    continue
                assert len(diff) > 1
                if rule == SIZE_INDEX or self.checked_length == LENGTH_SPECIFIED:
                    new_checked_length = LENGTH_SPECIFIED
                else:
                    new_checked_length = minl if rule >= self.checked_length else self.checked_length
                potential_kids = {key: Node(value, next_rules, new_checked_length)
                                  for key, value in diff.items()}
                if self.complexity_with_kids(potential_kids) < min_complexity:
                    self.kids = potential_kids
                    self.rule = rule

        if self.rule == SIZE_INDEX or self.checked_length == LENGTH_SPECIFIED:
            self.length_check = False
            self.checked_length = LENGTH_SPECIFIED
        else:
            if self.rule >= self.checked_length:
                self.length_check = True
                self.checked_length = minl

        assert not self.is_leaf()

    def is_leaf(self):
        return not self.kids

    def complexity_with_kids(self, kids):
        assert kids
        return sum([kid.complexity() for kid in kids.values()]) / len(self.strings) + 1

    def complexity(self):
        if self.is_leaf():
            return 0
        return self.complexity_with_kids(self.kids)
