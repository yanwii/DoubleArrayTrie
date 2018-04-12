#ifndef DAT
#define DAT

#include <iostream>
#include <unordered_map>
#include <deque>
using namespace std;


typedef struct Siblings{
    bool is_end = false;
    bool is_begin = false;
    int code;
    int parent_state;
    string parent_seg;
    string word;
} Node;

typedef unordered_map<string, deque<Node>> siblings_def;
typedef deque<deque<string>> cut_seg_def;

class DoubleArrayTrie{
    public:
    DoubleArrayTrie(){};
    void make_ac(deque<string>);
    int find_begin(deque<Node>);
    int prefix_search(string);
    void loop_map(unordered_map<string, int>);
    void init_storage();
    void reallocate_storage(int);
    deque<string> sibling_words;
    int get_parent_state(string);
    void print();
    cut_seg_def cut_seg(const deque<string> &);
    siblings_def fetch_siblings(int, cut_seg_def);
    // int base[100] = {0};
    // int check[100] = {0};
    deque<int> base;
    deque<int> check;
    deque<string> charl;

    int next_check_pos = 0;
    int nums_word = 0;
    int alloc_size = 1000;
    int max_col = 0;
    int max_index = 0;
    unordered_map<string, int> vocab;
};
#endif