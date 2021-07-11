from generation.rules import *
from copy import deepcopy

class Node:
    def __init__(self, strings, prev_rules: set):
        assert strings
        self.strings = strings
        self.kids: list(Node) = dict()
        self.rule = None
        self.prev_rules = list(prev_rules)

        if len(self.strings) == 1:
            return

        minl = min([len(s) for s in strings])
        min_complexity = len(strings) * (minl + 1)
        for i in range(-1, minl):
            if i not in prev_rules:
                next_rules = deepcopy(prev_rules)
                next_rules.add(i)
                assert len(strings) > 1
                diff = differentiate_by_one_rule(strings, i)
                if len(diff) == 1:
                    continue
                assert len(diff) > 1
                potential_kids = {key: Node(value, next_rules) for key, value in diff.items()}
                if self.complexity_with_kids(potential_kids) < min_complexity:
                    self.kids = potential_kids
                    self.rule = i

        assert not self.is_leaf()

    def is_leaf(self):
        return not self.kids

    def complexity_with_kids(self, kids):
        print(self.strings, kids)
        assert kids
        return sum([kid.complexity() for kid in kids.values()]) / len(self.strings) + 1

    def complexity(self):
        if self.is_leaf():
            return 0
        return self.complexity_with_kids(self.kids)
