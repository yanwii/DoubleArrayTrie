#ifndef DAT
#define DAT

#include <iostream>
#include <unordered_map>
#include <vector>
#include "trie_tree.h"
using namespace std;

class DoubleArrayTrie:TrieTree{
    public:
    DoubleArrayTrie(){};
    void make_ac();
    bool find_begin(const Node* root, int begin, deque<Node*> &queue, int *check);
    void prefix_search(string);
};
#endif