#include <iostream>
#include <vector>
#include <deque>
#include "double_array_trie.h"
#include "utils.h"
#include <math.h>
#include <string>
#include <time.h>
#include <deque>
using namespace std;

void pause(){
    char inputs[5];
    cout << "pause press c to continue ";
    cin >> inputs;
}

cut_seg_def DoubleArrayTrie::cut_seg(deque<string> &list){
    deque<deque<string>> segments;
    while(!list.empty()){
        string word = list.front();
        list.pop_front();
        try{
            deque<string> cut_seg = cut(word, false);
            max_col = cut_seg.size() > max_col ? cut_seg.size() : max_col;
            segments.push_back(cut_seg); 
        } catch(exception &e){
            cout << e.what() << endl;
        }
    }
    return segments;
}

void DoubleArrayTrie::reallocate_storage(int new_size){
    cout << "reallocate" << endl;
    for (int i=0; i<(new_size - alloc_size); i++){
        base.push_back(0);
        check.push_back(0);
    }
    print();
    alloc_size = new_size;
}

void DoubleArrayTrie::init_storage(){
    base = deque<int> (alloc_size, 0);
    check = deque<int> (alloc_size, 0);
    // charl = deque<string> (alloc_size, "");
}

void DoubleArrayTrie::make_ac(deque<string> list){
    // storage allocation
    init_storage();
    // deque<string> list = {"he" ,"his", "she", "her", "hers"};
    cut_seg_def segments = cut_seg(list);
    int col = 0;
    int max_col = 20;
    clock_t start, stop;
    while (col < max_col){
        // Loop the segments then adding word to d while 
        // constructing the base & check.
        siblings_def siblings_map = fetch_siblings(col, segments);

        if (col == 0) { col++; continue; }
        if (siblings_map.empty()) { break; }
        int local_max_index = 0;
        for(auto it=siblings_map.begin(); it!=siblings_map.end(); it++){
            string parent_word = it->first;
            deque<Node> siblings = it->second;
            start = clock();
            int begin = find_begin(siblings);
            for (Node node:siblings) {
                string word = node.word;
                int code = node.code;
                int t = code + begin;
                int s = node.parent_state;
                check[t] = s;
                // charl[t] = word;
                base[s] = begin;
            }
        }
        cout << "col: " << col << endl;
        col ++;
    }
    // perfct list base
    // if the word is the end of a segment but there are words atfer it(e.g.: he her):
    //  base[i] = -base[i]
    // else
    //  base[i] = -i
    while(!segments.empty()){
        deque<string> seg = segments.front();
        segments.pop_front();
        string word = seg.front();
        string sent = word;
        seg.pop_front();
        int p = vocab[word];
        int b = 0;
        int bp = 0;
        while(!seg.empty()){
            word = seg.front();
            sent += word;
            seg.pop_front();
            b = abs(base[p]) + vocab[word];
            p = b;
        }
        bp = base[p];
        if (bp > 0) { base[p] = -base[p];}
        else if (bp == 0) { base[p] = -1;} 
    }
    print();
}

void DoubleArrayTrie::print(){
    for (int i=0; i<30 ; i++){
        cout << i;
        cout << " ";
        cout << base[i];
        cout << " ";
        cout << check[i];
        cout << " " << endl;
        // cout << charl[i] << endl;
    }
    cout << "-------------" << endl;
}

int DoubleArrayTrie::get_parent_state(string seg){
    deque<string> to_searchl = cut(seg, false); 
    string word = to_searchl.front();
    to_searchl.pop_front();
    int p = vocab[word];
    int b = 0;
    while(!to_searchl.empty()){
        word = to_searchl.front();
        to_searchl.pop_front();
        p = abs(base[p]) + vocab[word];
    }
    return p;
}

siblings_def DoubleArrayTrie::fetch_siblings(int col, cut_seg_def segments){
    // collect siblings and figure out begin
    siblings_def siblings_map;
    string parent_seg = "";
    while(!segments.empty()){
        deque<string> seg = segments.front();
        segments.pop_front();
        if (col >= seg.size()) { continue; }
        string word = seg[col];
        vocab[word] = vocab[word] == 0 ? ++nums_word : vocab[word];
        
        if (col == 0) { 
            check[vocab[word]] = 1;
            continue;
        } else {
            string parent_word = seg[col - 1];
            string key = parent_word + "_" + word;
            Node node;
            for (int i=0; i<col; i++) { parent_seg += seg[i];}
            node.code = vocab[word];
            node.word = word;
            node.parent_state = get_parent_state(parent_seg);
            siblings_map[parent_word].push_back(node);
            parent_seg = "";
        }
    }
    return siblings_map;
}

int DoubleArrayTrie::find_begin(deque<Node> siblings){
    int pos = siblings[0].code + 1 > max_index ? siblings[0].code + 1: max_index;
    bool is_found = true;
    int begin = 0;
    int first = 0;
    while(true){
        for (Node node: siblings){
            begin = pos - siblings[0].code;
            int code = node.code;
            if (code + begin > alloc_size) { cout << "leak" << endl;}
            if (base[code + begin] !=0 || check[code + begin] !=0){
                is_found = false;
                break;
            } else if (first == 0){
                max_index = pos;
                first == 1;
            }

        }
        if (!is_found){
            pos++;
            is_found = true;
        } else {
            return begin;
        }
    }

}

int DoubleArrayTrie::prefix_search(string to_search){
    deque<string> to_searchl = cut(to_search, false);
    string word = to_searchl.front();
    to_searchl.pop_front();
    int p = vocab[word];
    int b = 0;
    string result = word;
    while(!to_searchl.empty()){
        word = to_searchl.front();
        to_searchl.pop_front();
        b = abs(base[p]) + vocab[word];
        int bp = base[p];
        if (bp < 0) { 
            cout << result << endl;
        }
        cout << check[b] << " " << p << " " << b << endl;
        if (check[b] == p || abs(base[check[b]]) + vocab[word] == b){
            p = b;
            result += word;
            continue;
        }
        return 0;
    }
    cout << result << endl;
}

void DoubleArrayTrie::loop_map(unordered_map<string, int> map){
    for (auto it=map.begin(); it!=map.end(); it++){
        cout << it->first << ":" << it->second << endl;
    }
}

int main(){
    DoubleArrayTrie dat;
    deque<string> company = read_file("test");
    // deque<string> company = {"he" ,"her", "his", "se", "she", "hers"};
    // deque<string> company = {"阿拉伯人去哪里", "重庆人啊去哪里"};
    time_t start, stop;

    start = time(NULL);
    dat.make_ac(company);
    stop = time(NULL);
    cout << "cost: " << stop - start << endl; 
    dat.prefix_search("河北捷成建设造价咨询有限公司hers");
}

