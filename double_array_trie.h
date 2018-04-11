#ifndef DAT
#define DAT

#include <iostream>
#include <unordered_map>
#include <deque>
using namespace std;


typedef struct Siblings{
    bool is_end = false;
    bool is_begin = false;
    int parent_state;
    int code;
    string parent_word;
    string word;
} Node;

typedef deque<Node> siblings_def;
typedef deque<deque<string>> cut_seg_def;

class DoubleArrayTrie{
    public:
    DoubleArrayTrie(){};
    void make_ac(deque<string>);
    int find_begin(string word, int);
    void prefix_search(string);
    void loop_map(unordered_map<string, int>);
    void init_storage();
    void reallocate_storage(int);
    int get_parent_state(string);
    void print();
    cut_seg_def cut_seg(const deque<string> &);
    siblings_def count_siblings(int, cut_seg_def);
    // int base[100] = {0};
    // int check[100] = {0};
    deque<int> base;
    deque<int> check;
    vector<string> charl;

    int next_check_pos = 0;
    int nums_word = 0;
    int alloc_size = 1;
    unordered_map<string, int> vocab;
};
#endif