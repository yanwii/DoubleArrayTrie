objects = double_array_trie.o utils.o

double_array_trie: $(objects)
	$(CPP) $(CPPFLAGS) -o double_array_trie $(objects)

double_array_trie.o: 
utils.o:

.PHONY: clean
clean:
	rm $(objects)

CPP=g++
CPPFLAGS=-std=c++11 -g