#ifndef DAT
#define DAT

#include <iostream>
#include <unordered_map>
#include <deque>
#include "trie_tree.h"
using namespace std;

typedef unordered_map<string, deque<string>> sibling_map_def;
typedef deque<deque<string>> cut_seg_def;

class DoubleArrayTrie:TrieTree{
    public:
    DoubleArrayTrie(){};
    void make_ac(deque<string>);
    bool find_begin(deque<string>, int);
    void prefix_search(string);
    void loop_map(unordered_map<string, int>);
    void init_storage();
    void reallocate_storage(int);
    cut_seg_def cut_seg(const deque<string> &);
    sibling_map_def count_siblings(int, cut_seg_def);
    void clear_map(int);
    // int base[100] = {0};
    // int check[100] = {0};
    vector<int> base;
    vector<int> check;

    int next_check_pos = 0;
    int nums_word = 0;
    int alloc_size = 1;
    unordered_map<string, int> vocab;
    unordered_map<int, unordered_map<string, int>> pre_state;
};
#endif