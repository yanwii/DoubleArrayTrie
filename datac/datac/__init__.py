import datac

class DatAc(object):
    def __init__(self):
        self.ac = datac.DoubleArrayTrie()
        print self.ac.num_words
    