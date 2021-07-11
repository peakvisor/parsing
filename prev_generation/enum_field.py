from generation.rules import SIZE_INDEX

CHECK_SUBSTR = False

class EnumField:
    def __init__(self, name, string):
        self.name = name
        self.string = string
        self.known_fields = []

    def memcmp_check(self, check_start: int = 0):
        substr = "" if check_start == 0 else (" + " + str(check_start))
        return '!std::memcmp(string.begin()' + substr + ', "' \
               + self.string[check_start:] + '", ' + str(len(self.string) - check_start) + ")"

    def eq_check(self, check_start: int = 0):
        substr = "" if check_start == 0 else (".substr(" + str(check_start) + ")")
        return 'string' + substr + ' == "' + self.string[check_start:] + '"'

    def check(self):
        check_start = self.first_unknown_field() if CHECK_SUBSTR else 0
        assert check_start >= 0
        return self.memcmp_check(check_start) if self.size_is_known() \
            else self.eq_check(check_start)

    def return_string_or_empty(self):
        return "return " + self.check() + " ? " + self.name + " : E{};"

    def check_return(self):
        return "if (" + self.check() + ") return " + self.name + ";"

    def size_is_known(self):
        return self.known_fields and self.known_fields[0] == SIZE_INDEX

    def first_unknown_field(self):
        last_known = -1
        for n in self.known_fields:
            if n > last_known + 1:
                return last_known + 1
            else:
                last_known = n
        return last_known + 1
