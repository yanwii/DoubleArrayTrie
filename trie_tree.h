#ifndef TRIETREE
#define TRIETREE

#include <iostream>
#include <vector>
#include <unordered_map>
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
    }
    Node* proot;
    int node_nums=0;
    public:
    void add_words(vector<string> &word, bool);
    void add_word(string word, bool=false);
};
#endif