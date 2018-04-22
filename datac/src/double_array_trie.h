#ifndef DAT
#define DAT

#include <iostream>
#include <unordered_map>
#include <vector>
#include <deque>
using namespace std;


typedef struct Siblings{
    int code;
    wstring seg;
    int col;
    wstring word;
} Node;

typedef unordered_map<wstring, vector<Node>> siblings_def;

class DoubleArrayTrie{
    private:
    vector<int> prefix_search(string&);
    vector<int> prefix_search(const wstring&);
    public:
    DoubleArrayTrie(){};
    ~DoubleArrayTrie(){};

    template<class T>
    void STL_clear(T&);

    void make_ac(vector<wstring>&);
    void make_ac(vector<string>&);
    int find_begin(vector<Node>&);
    vector<string> common_prefix_search(string&);
    void loop_map(unordered_map<wstring, int>);
    vector<string> search(const wstring&);
    vector<string> search(string&);
    void init_storage();
    void reallocate_storage(int);
    int get_parent_state(wstring);
    void print();
    void fetch_siblings(vector<Node>&, deque<vector<Node>>&);
    void fetch_siblings(vector<wstring>&, deque<vector<Node>>&);
    // int base[100] = {0};
    // int check[100] = {0};
    vector<int> base;
    vector<int> check;

    int next_check_pos = 0;
    int num_words = 0;
    int alloc_size = 1;
    int max_col = 0;
    int max_index = 0;
    unordered_map<wstring, int> vocab;
};
#endif