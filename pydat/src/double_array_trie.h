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
    int find_begin(vector<Node>&);
    template<class T>
    void STL_clear(T&);
    void init_storage();
    void reallocate_storage(int);
    void fetch_siblings(vector<Node>&, deque<vector<Node>>&);
    void fetch_siblings(vector<wstring>&, deque<vector<Node>>&);
    int get_parent_state(const wstring&);
    vector<string> search(const wstring&);
    void add_word(const wstring&);

    vector<wstring> segments;
    vector<int> base;
    vector<int> check;
    unordered_map<wstring, int> vocab;
    int next_check_pos = 0;
    int alloc_size = 1000;
    int max_col = 0;
    int max_index = 0;
    
    public:
    DoubleArrayTrie(){};
    ~DoubleArrayTrie(){};
    
    void make();
    vector<string> common_prefix_search(string);
    vector<string> search(string);
    void add_word(string);
    void add_words(vector<string>);
    void add_words(vector<wstring>);

    int num_words = 0;
};
#endif