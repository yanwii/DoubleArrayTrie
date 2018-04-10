#include <iostream>
#include <vector>
#include <deque>
#include "trie_tree.h"
#include "double_array_trie.h"
#include "utils.h"
#include <math.h>
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
    for (int i=0; i<(new_size - alloc_size); i++){
        base.push_back(0);
        check.push_back(0);
    }
    alloc_size = new_size;
}

void DoubleArrayTrie::init_storage(){
    base = vector<int> (alloc_size);
    check = vector<int> (alloc_size);
}

void DoubleArrayTrie::make_ac(deque<string> list){
    // storage allocation
    init_storage();

    // deque<string> list = {"he" ,"his", "she", "her", "hers"};
    cut_seg_def segments = cut_seg(list);

    int col = 0;
    int max_col = 20;
    sibling_map_def siblings;
    while (true){
        // Loop the segments then adding word to vocab while 
        // constructing the base & check.
        if (col == 0) { count_siblings(col, segments); col++; continue;}
        siblings = count_siblings(col, segments);
        if (siblings.empty()) { break; }
        // Find begin first
        for (auto it=siblings.begin(); it!=siblings.end(); it++){
            string pre_word = it->first;
            deque<string> seg = it->second;
            int begin = find_begin(seg, 1);
            // base & check
            int s = pre_state[col-1][pre_word];

            for (string word : seg){
                int t = vocab[word] + begin;
                if (t >= alloc_size) { reallocate_storage(t); }
                check[t] = s;
                pre_state[col][word] = t;
            }
            base[s] = begin;
        }
        clear_map(col);
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
    cout << base.size() << endl;
}

void DoubleArrayTrie::clear_map(int col){
    pre_state.erase(col-1);
}

sibling_map_def DoubleArrayTrie::count_siblings(int col, cut_seg_def segments){
    sibling_map_def siblings;
    for (deque<string> seg : segments) { 
        if (col >= seg.size()) { continue;}
        string cur_word = seg[col];
        // if col equals 0
        //    init pre_state & charl 
        if (col == 0) { 
            vocab[cur_word] = vocab[cur_word] == 0 ? ++nums_word : vocab[cur_word]; 
            pre_state[col][cur_word] = vocab[cur_word];
            continue; 
        }
        string pre_word = seg[col-1];
        // add to vocab
        if (vocab[cur_word] == 0){
            nums_word ++;
            vocab[cur_word] = nums_word;
        }
        // collect sibling
        siblings[pre_word].push_back(seg[col]);
    }
    return siblings;
}

bool DoubleArrayTrie::find_begin(deque<string> seg, int begin){
    for (string word : seg) {
        if ( vocab[word] + begin >= alloc_size) { reallocate_storage(vocab[word] + begin); }
        if (base[vocab[word] + begin]!=0 || check[vocab[word] + begin]!=0){
            return find_begin(seg, ++begin);
        }
    }
    return begin;
}

void DoubleArrayTrie::prefix_search(string to_search){
    deque<string> to_searchl = cut(to_search, false);
    int p = 0;
    string result = "";
    while(!to_searchl.empty()){
        string word = to_searchl.front();
        to_searchl.pop_front();
        result += word;
        p = abs(base[p]) + vocab[word];
        int bp = base[p];
        if (bp < 0) { 
            cout << result << endl;
        }
    }
}

void DoubleArrayTrie::loop_map(unordered_map<string, int> map){
    for (auto it=map.begin(); it!=map.end(); it++){
        cout << it->first << ":" << it->second << endl;
    }
}

int main(){
    DoubleArrayTrie dat;
    deque<string> company = read_file("~/SocialCredits/CompanyName/company_names.txt");
    dat.make_ac(company);
    dat.prefix_search("hers");
}

