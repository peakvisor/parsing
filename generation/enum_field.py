from rules import SIZE_INDEX

class EnumField:
    def __init__(self, name, string):
        self.name = name
        self.string = string
        self.known_fields = []
        self.size_is_known = False

    def memcmp_check(self):
        return '!std::memcmp(string.begin(), "' + self.string + '", ' + str(len(self.string)) + ")"

    def eq_check(self):
        return 'string == "' + self.string + '"'

    def check(self):
        assert self.size_is_known == (SIZE_INDEX in self.known_fields)
        return self.memcmp_check() if self.size_is_known else self.eq_check()

    def return_string_or_empty(self):
        return "return " + self.check() + " ? " + self.name + " : E{};"

    def check_return(self):
        return "if (" + self.check() + ") return " + self.name + ";"
