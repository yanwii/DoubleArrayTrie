#ifndef DAT
#define DAT

#include <iostream>
#include <unordered_map>
#include <deque>
using namespace std;


typedef struct Siblings{
    int code;
    int parent_state;
    wstring word;
} Node;

typedef unordered_map<wstring, vector<Node>> siblings_def;
typedef vector<vector<string>> cut_seg_def;

class DoubleArrayTrie{
    private:
    vector<int> prefix_search(string &);
    public:
    DoubleArrayTrie(){};
    ~DoubleArrayTrie(){};

    template<class T>
    void STL_clear(T&);

    void make_ac(vector<wstring> &);
    void make_ac(vector<string> &);
    int find_begin(vector<Node>);
    vector<string> common_prefix_search(string&);
    void loop_map(unordered_map<wchar_t, int>);
    void init_storage();
    void reallocate_storage(int);
    int get_parent_state(wstring);
    void print();
    cut_seg_def cut_seg(vector<string> &);
    siblings_def fetch_siblings(int, vector<wstring>&);
    // int base[100] = {0};
    // int check[100] = {0};
    vector<int> base;
    vector<int> check;
    vector<int> chari;

    int next_check_pos = 0;
    int nums_word = 0;
    int alloc_size = 50000000;
    int max_col = 0;
    int max_index = 0;
    unordered_map<wchar_t, int> vocab;
};
#endif