from . import pydat

class Dat(object):
    def __init__(self):
        self.__dat_c = pydat.DoubleArrayTrie()

    def add_word(self, word):
        self.__dat_c.add_word(word)

    def make(self):
        self.__dat_c.make()

    def greedy_search(self, to_search):
        result = self.__dat_c.greedy_search(to_search)
        return self.__format_output(result, to_search)

    def search(self, to_search):
        result = self.__dat_c.search(to_search)
        return self.__format_output(result, to_search)

    def prefix_search(self, to_search):
        result = self.__dat_c.common_prefix_search(to_search)
        return self.__format_output(result, to_search)

    def load_file(self, file_name):
        self.__dat_c.load_file(file_name)

    def __format_output(self, result, to_search):
        try:
            if isinstance(to_search, str):
                to_search = to_search.decode("utf-8")
        except Exception:
            pass
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

