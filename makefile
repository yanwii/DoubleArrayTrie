objects = double_array_trie.o trie_tree.o utils.o

double_array_trie: $(objects)
	$(CPP) $(CPPFLAGS) -o double_array_trie $(objects)

double_array_trie.o:trie_tree.h 
trie_tree.o:trie_tree.h utils.h
utils.o:utils.h

.PHONY: clean
clean:
	rm $(objects)

CPP=g++
CPPFLAGS=-std=c++11