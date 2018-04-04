#include <iostream>
#include <vector>
#include <deque>
#include "trie_tree.h"
#include "double_array_trie.h"
#include "utils.h"
#include <deque>
using namespace std;

void DoubleArrayTrie::make_ac(){
    // 构建 base 和 che ck 数组
    
    deque<string> list = {"茶叶", "茶树", "走廊"};
    add_words(list, false);

    deque<Node*> queue;
    Node* root = proot;

    queue.push_back(root);
    unordered_map<string, int> parent_begin;
    
    while(!queue.empty()){
        root = queue.front();
        queue.pop_front();
        int parent_state = root->state;
        //find begin
        int begin = 0;
        bool is_found = true;
        string cur;
        while(true){
            begin ++;
            is_found = find_begin(root, begin, queue, check);
            // 判断是否找到合适的begin
            if (is_found){
                cur = root->word + "_" + to_string(root->depth);
                if (parent_begin[cur] != 0){ 
                    base[parent_begin[cur] + vocab[root->word]] = begin; 
                }
                for (auto it=root->child.begin(); it!=root->child.end(); it++){
                    Node* node = it->second;
                    cout << node->word << endl;
                    queue.push_back(node);
                    check[begin + vocab[node->word]] = begin;
                    charl[begin + vocab[node->word]] = node->word;
                    if (node->word == "end"){
                        base[begin + vocab[node->word]] = -1;
                    } else {
                        cur = node->word + "_" + to_string(node->depth);
                        parent_begin[cur] = begin;
                    }
                }
                break;
            }
        }
    }
    for (int i=0; i<20; i++){
        cout << base[i] << " " << check[i] << " " << charl[i] << endl;
    }

    for (auto it_=vocab.begin(); it_!=vocab.end(); it_++){
        cout << it_->first << ":" << it_->second << endl;
    }
}

bool DoubleArrayTrie::find_begin(const Node* root, int begin, deque<Node*> &queue, int *check){
    bool is_found = true;
    for (auto it=root->child.begin(); it!=root->child.end(); it++){
        Node* node = it->second;
        if (check[begin + vocab[node->word]] != 0){
            is_found = false;
            break;
        }
    }
    return is_found;
}

void DoubleArrayTrie::prefix_search(string to_search){
    deque<string> to_searchl = cut(to_search, false);
    int p = 0;
    while(!to_searchl.empty()){
        string word = to_searchl.front();
        to_searchl.pop_front();

        p = base[p] + vocab[word] + 1;
        int bp = base[p];
        int bbp = base[bp];
        int cbp = check[bp];
        cout << word << " p " << p << " bp "<< bp << " bbp " << bbp << " cbp " << cbp << endl; 
    }
}

int main(){
    DoubleArrayTrie dat;
    dat.make_ac();
    dat.prefix_search("茶树");
}