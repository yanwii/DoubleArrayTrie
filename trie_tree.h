#ifndef TRIETREE
#define TRIETREE

#include <iostream>
#include <vector>
#include <unordered_map>
#include <deque>

using namespace std;

typedef struct ChildNode{
    public:
    string word;
    unordered_map<string, ChildNode*> child;
    int state=0;
    int depth;
    bool is_end = false;
    string segment;
} Node;

class TrieTree{
    public:
    TrieTree(){
        proot = new Node();
        vocab["end"] = 1;
    }
    Node* proot;
    int node_nums=1;
    unordered_map <string, int> vocab;
    void add_words(deque<string> &word, bool);
    void add_word(string word, bool=false);
};
#endif