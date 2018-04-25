## DoubleArrayTrie

DoubleArrayTrie(DAT) support prefix search & exact search & multiple pattern match for python implemented by c++

### C++

    #include "double_array_trie.h"
    
    DoubleArrayTrie dat;
    // load from file
    dat.load_file("test.txt")
    // or add word
    dat.add_word("he")
    dat.add_word("she")
    // make base & check
    dat.make()
    // search
    vector<string> index_s = dat.search("ushers")

### Python

    # install 
    pip install pydat

    # use
    dat = pydat.Dat()
    dat.load_file("test.txt")
    dat.add_word("he")
    dat.make()
    dat.search("ushers")


## About DAT
![](https://linux.thai.net/~thep/datrie/double.gif)

**Definition**  
check[base[s] + c] = s  
base[s] + c = t  


Reference: [An implementation of Double-Array Trie](https://linux.thai.net/~thep/datrie/datrie.html#Double)