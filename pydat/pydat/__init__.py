import pydat

class Dat(object):
    def __init__(self):
        self.dat_c = pydat.DoubleArrayTrie()

    def add_word(self, word):
        self.dat_c.add_word(word)

    def make(self):
        self.dat_c.make()

    def search(self, to_search):
        result = self.dat_c.search(to_search)
        return self.format_output(result, to_search)

    def prefix_search(self, to_search):
        result = self.dat_c.common_prefix_search(to_search)
        return self.format_output(result, to_search)

    def load_file(self, file_name):
        self.dat_c.load_file(file_name)

    def format_output(self, result, to_search):
        if isinstance(to_search, str):
            to_search = to_search.decode("utf-8")
        entities = []
        for index_s in result:
            try:
                begin, end = index_s.split("_")
                begin = int(begin)
                end = int(end)
                tmp = {
                    "begin":begin,
                    "end":end,
                    "entity":to_search[begin:end]
                }
                entities.append(tmp)
            except Exception as error:
                continue
        return { "entities": entities }

