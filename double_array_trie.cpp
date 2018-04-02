#include <iostream>
#include <vector>
#include "trie_tree.h"
#include "double_array_trie.h"
#include <deque>
using namespace std;

void DoubleArrayTrie::make_ac(){
    cout << "pass" << endl;
}

int main(){
    vector<string> list = {"一举", "一举一动", "一举成名", "一举成名天下知", "万能", "万能胶"};
    TrieTree tt;
    tt.add_words(list, false);

    deque<Node*> queue;
    Node* root = tt.proot;
    queue.push_back(root);
    
    while(!queue.empty()){
        root = queue.back();
        queue.pop_back();
        int parent_state = root->state;
        for (auto it=root->child.begin(); it!=root->child.end(); it++){
            string word = it->first;
            Node* node = it->second;
            queue.push_back(node);

            cout << "word " << word << " state " << node->state;
            cout << " parent_state " << parent_state << endl;
        }
    }
}