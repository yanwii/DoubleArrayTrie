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
    int base[100] = {1};
    int check[100] = {0};
    
    deque<string> list = {"一举", "一举一动", "一举成名", "一举成名天下知", "万能", "万能胶"};
    add_words(list, false);

    deque<Node*> queue;
    Node* root = proot;
    queue.push_back(root);
    
    while(!queue.empty()){
        root = queue.front();
        queue.pop_front();
        int parent_state = root->state;
        //find begin
        int begin = 0;
        bool is_found = true;

        while(true){
            begin ++;
            is_found = find_begin(root, begin, queue, check);
            // 判断是否找到合适的begin
            if (is_found){
                for (auto it=root->child.begin(); it!=root->child.end(); it++){
                    Node* node = it->second;
                    queue.push_back(node);
                    check[begin + vocab[node->word]] = begin;
                }
                if (root->word == "end"){
                    base[begin + vocab[root->word]] = -1;
                } else {
                    base[begin + vocab[root->word]] = begin;
                }
                break;
            }
        }
    }
    for (int i=0; i<30; i++){
        cout << base[i] << " " << check[i] << endl;
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
}

int main(){
    DoubleArrayTrie dat;
    dat.make_ac();
    dat.prefix_search("万能胶");
}