#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <exception>
#include "utils.h"
#include "trie_tree.h"
using namespace std;


void TrieTree::add_words(deque<string> &words, bool if_reverse=false){
    /*
    Add words to Trie Tree

    args:
        words     :  vector<string> 
                     like {"she", "he"}
        if_reverse:  bool
                     if reverse the segment
    */
    while(!words.empty()){
        string word = words.back();
        words.pop_back();
        try{
            add_word(word, if_reverse);
        } catch (exception &e){
            continue;
        }
    }
}


void TrieTree::add_word(string word, bool if_reverse){
    /*
    Add word to Trie Tree

    args:
        words:      vector<string>;
                    like {"s", "h", "s"}
        if_reverse: bool;
                    if reverse the segment
    */
    try {
        deque<string> words = cut(word, if_reverse);

        //root node
        Node* root = proot; 
        string tmp = "";

        //reverse
        for (int i=0; i<words.size(); i++){
            string word = words[i];
            tmp += words[i];
            // check if the node exist
            if (root->child[word]==NULL){
                // if not exist
                // add new node
                if (vocab.find(word) == vocab.end()){
                    node_nums++;
                    vocab[word] = node_nums;
                }
                root->child[word] = new Node();
                root->child[word]->word = word;
                root->child[word]->state = vocab[word];
                root->child[word]->depth = i+1;
            };
            // goto child node
            root = root->child[word];
        }
        root->is_end = true;
        root->segment = tmp;
        root->child["end"] = new Node();
        root->child["end"]->word = "end";
        root->child["end"]->state = 1;
        root->child["end"]->depth = words.size() + 1;
    } catch (exception &e){}
};





