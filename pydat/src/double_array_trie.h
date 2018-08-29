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
    void fetch_siblings(deque<vector<Node>>&);
    int get_parent_state(const wstring&);
    vector<string> greedy_search(const wstring&);
    vector<string> search(const wstring&);
    vector<int> maxmum_search(string&);
    vector<int> maxmum_search(const wstring&);

    unordered_map<wstring, int> vocab;
    int num_chars = 0;
    vector<wstring> segments;
    vector<int> base;
    vector<int> check;
    int max_index = 0;
    int alloc_size = 5000;
    int num_words = 0;

    public:
    DoubleArrayTrie(){};
    ~DoubleArrayTrie(){};
    
    void make();
    void set_alloc(int);
    vector<string> common_prefix_search(string);
    vector<string> greedy_search(string);
    vector<string> search(string);
    void add_word(string);
    void add_word(const wstring&);
    void add_words(vector<string>);
    void add_words(vector<wstring>);
    void load_file(const string&);
    
};
#endif