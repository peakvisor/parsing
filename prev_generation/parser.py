from copy import deepcopy

NOT_FINISHED = -1
PARSING_ERROR = -2

class Replacement:
    def __init__(self):
        self.lines_to_replace = []
        self.replacement_lines = []

class BracketStack:
    def __init__(self, brackets):
        self.closing_brackets = {brackets[2*i+1]: brackets[2*i] for i in range(len(brackets) // 2)}
        self.opening_brackets = {x for x in brackets[::2]}
        self.stack = []

    def add_line(self, line, start=0):
        if not self.stack:
            assert(line[start] in self.opening_brackets)
            self.stack.append(line[start])
            start += 1
        for i in range(start, len(line)):
            assert self.stack
            if line[i] in self.closing_brackets:
                if self.stack[-1] == self.closing_brackets[line[i]]:
                    self.stack.pop()
                    if not self.stack:
                        return i
                else:
                    return PARSING_ERROR
            elif line[i] in self.opening_brackets:
                self.stack.append(line[i])
        return NOT_FINISHED

class CodeBlock:
    def __init__(self, src, start_line, end_line=-1):
        self.src = src
        self.start = start_line
        if end_line == -1:
            opening_bracket = self.src[start_line].rfind("{")
            assert opening_bracket != -1
            closing_bracket = find_closing_bracket(src, start_line, opening_bracket)
            assert closing_bracket not in [NOT_FINISHED, PARSING_ERROR]
            self.end = closing_bracket[0] + 1
        else:
            self.end = end_line

    def to_str(self):
        string = ""
        for i in range(self.start, self.end):
            string += self.src[i] + "\n"
        return string

    def insides_to_str(self):
        string = ""
        for i in range(self.start + 1, self.end - 1):
            string += self.src[i]
        return string

    def find_sub_block(self, sub_block_marker):
        for i in range(self.start + 1, self.end):
            if sub_block_marker in self.src[i]:
                method_block = CodeBlock(self.src, i)
                return method_block
        return CodeBlock(self.src, self.end - 1, self.end - 1)


def strip_comments(s):
    for i in range(len(s)):
        s[i] = s[i].rstrip()
        comment_start = s[i].find("//")
        if comment_start != -1:
            s[i] = s[i][:comment_start]

def find_closing_bracket(src, line_index, opening_bracket_index):
    bs = BracketStack("{}")
    return_code = bs.add_line(src[line_index], opening_bracket_index)
    for i in range(line_index + 1, len(src) + 1):
        if return_code == PARSING_ERROR:
            print("parsing error finding closing bracket from index" + opening_bracket_index + " in")
            print(src[line_index])
            assert False
        if return_code != NOT_FINISHED:
            return i - 1, return_code
        if i == len(src):
            return PARSING_ERROR
        return_code = bs.add_line(src[i])
    return PARSING_ERROR

def find_mappings(src):
    mappings = []
    src_copy = deepcopy(src)
    strip_comments(src_copy)
    for line_index in range(len(src_copy)):
        line = src_copy[line_index]
        if "struct" in line and "Mapping" in line:
            m = CodeBlock(src_copy, line_index)
            mappings.append(m)
    return mappings

def parse_values(values_string):
    bs = BracketStack("{}")
    cursor = 0
    enum = []
    while True:
        opening_bracket = values_string.find("{", cursor)
        if opening_bracket == -1:
            break
        closing_bracket = bs.add_line(values_string, opening_bracket)
        cursor = closing_bracket + 1
        assert closing_bracket not in [NOT_FINISHED, PARSING_ERROR]
        coma = values_string.rfind(",", opening_bracket, closing_bracket)
        enum_value = values_string[opening_bracket + 1: coma].strip()

        opening_quote = values_string.find('"', coma)
        closing_quote = values_string.find('"', opening_quote + 1)

        enum_string = values_string[opening_quote + 1: closing_quote]
        enum.append((enum_value, enum_string))
    return enum

def get_mapping_name(s):
    start_marker = "struct "
    end_marker = "Mapping"
    start = s.find(start_marker) + len(start_marker)
    end = s.find(end_marker, start)
    if -1 in {start, end}:
        return None
    return s[start:end].lstrip()