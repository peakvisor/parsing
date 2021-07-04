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
