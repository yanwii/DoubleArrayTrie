#include <iostream>
#include <vector>
#include <deque>
#include "double_array_trie.h"
#include "utils.h"
#include <math.h>
#include <time.h>
#include <deque>
using namespace std;

cut_seg_def DoubleArrayTrie::cut_seg(const deque<string> &list){
    deque<deque<string>> segments;
    for (string word: list){ 
        try{
            segments.push_back(cut(word, false)); 
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
    charl = vector<string> (50000, " ");
}

void DoubleArrayTrie::make_ac(deque<string> list){
    // storage allocation
    init_storage();

    // deque<string> list = {"he" ,"his", "she", "her", "hers"};
    cut_seg_def segments = cut_seg(list);
    time_t start,stop;
    int col = 0;
    int max_col = 20;
    siblings_def siblings;
    while (true){
        // Loop the segments then adding word to vocab while 
        // constructing the base & check.
        siblings = count_siblings(col, segments);
        
        if (siblings.empty()) { break; }
        int t;
        int s;
        cout << "col: " << col << endl;
        // Find begin first
        while (!siblings.empty()){
            Node sibling = siblings.front();
            siblings.pop_front();
            string word = sibling.word;
            int begin = find_begin(word, 1);
            if (sibling.is_begin){
                t = vocab[word];
                check[t] = 1;
                charl[t] = word;
                continue;
            }
            // s = get_parent_state(sibling.parent_word);
            s = sibling.parent_state;
            t = vocab[word] + begin;
            // if (t >= alloc_size) { reallocate_storage(t); }
            check[t] = s;
            charl[t] = word;
            base[s] = begin;
            stop = time(NULL);
        }
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
        int p = 0;
        for(int i=0; i<seg.size(); i++){
            string word = seg[i];
            p = abs(base[p]) + vocab[word];
            int bp = base[p];
            if (bp == 0){ base[p]= -p;}
            if (i == seg.size()-1 && bp > 0) { base[p] = -base[p];}
        }
    }
    loop_map(vocab);
    print();
    cout << base.size() << endl;
}

void DoubleArrayTrie::print(){
    for (int i=0; i<20 ; i++){
        cout << i;
        cout << " ";
        cout << base[i];
        cout << " ";
        cout << check[i];
        cout << " ";
        cout << charl[i] << endl;
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

siblings_def DoubleArrayTrie::count_siblings(int col, cut_seg_def segments){
    siblings_def siblings;
    unordered_map<string, int> dup;
    for (deque<string> seg : segments) { 
        if (col >= seg.size()) { continue;}
        string cur_word = seg[col];
        // if col equals 0
        //    init pre_state & charl 
        Node sibling;
        vocab[cur_word] = vocab[cur_word] == 0 ? ++nums_word : vocab[cur_word]; 
        string parent_key = col == 0 ? "_" + cur_word : seg[col-1] + "_" + cur_word;
        if (dup[parent_key] != 0) { continue; } 
        dup[parent_key] = 1;
        sibling.code = vocab[cur_word];
        sibling.word = cur_word;
        sibling.is_begin = col == 0 ? true : false;
        string parent_seg = "";
        for (int i=0; i<col; i++) { parent_seg += seg[i];}
        sibling.parent_word = parent_seg;
        sibling.parent_state = col == 0 ? 0 : get_parent_state(parent_seg);
        siblings.push_back(sibling);
    }
    return siblings;
}

int DoubleArrayTrie::find_begin(string word, int begin){
    if (base[vocab[word] + begin]!=0 || check[vocab[word] + begin]!=0){
        return find_begin(word, ++begin);
    }
    return begin;
}

void DoubleArrayTrie::prefix_search(string to_search){
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
        if (check[b] != p){
            break;
        }
        p = b;
        result += word;
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
    deque<string> company = read_file("test.text");
    // deque<string> company = {"he" ,"her", "his", "se", "she", "hers"};
    // deque<string> company = {"啊", "埃及", "阿胶", "阿根廷", "阿拉伯", "阿拉伯人"};
    dat.make_ac(company);
    dat.prefix_search("阿拉伯人");
}

