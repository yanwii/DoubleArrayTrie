#include <iostream>
#include <vector>
#include "double_array_trie.h"
#include "utils.h"
#include <math.h>
#include <string>
#include <time.h>
#include <locale>
#include <deque>
#include <fstream>
#include <chrono>
#include <thread>
using namespace std;

void pause(){
    char inputs[5];
    cout << "pause press c to continue ";
    cin >> inputs;
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
    base = vector<int> (alloc_size, 0);
    check = vector<int> (alloc_size, 0);
}

void DoubleArrayTrie::make_ac(vector<string>& list){
    vector<wstring> wlist;
    for (int i=0; i<list.size(); i++){
        wlist.push_back(string_to_wstring(list[i]));
    }
    STL_clear(list);
    make_ac(wlist);
}

void DoubleArrayTrie::make_ac(vector<wstring>& list){
    // storage allocation
    init_storage();
    // deque<string> list = {"he" ,"his", "she", "her", "hers"};

    int col = 0;
    int max_col = 40;
    clock_t start, stop;
    // first collection
    deque<vector<Node>> queue;
    fetch_siblings(list, queue);

    while (!queue.empty()){
        // Loop the segments then adding word to d while 
        // constructing the base & check.
        vector<Node> siblings = queue.front();
        queue.pop_front();
        fetch_siblings(siblings, queue);

        int begin = find_begin(siblings);
        for (Node node:siblings){
            int code = node.code;
            int t = begin + code;
            int s = get_parent_state(node.seg.substr(0, node.col));
            check[t] = s;
            base[s] = begin;
        }
    }
    // perfct list base
    // if the word is the end of a segment but there are words atfer it(e.g.: he her):
    //  base[i] = -base[i]
    // else
    //  base[i] = -i
    for (int i=0; i<list.size(); i++){
        wstring seg = list[i];
        wstring word = wstring(1, seg[0]);
        int code = vocab[word];
        check[code] = 0;
        int p = code;
        int b = 0;
        int bp = 0;
        for (int j=1; j<seg.size(); j++){
            word = wstring(1, seg[j]);
            code = vocab[word];
            b = abs(base[p]) + code;
            p = b;
        }
        bp = base[p];
        if (bp > 0) { base[p] = -base[p];}
        else if (bp == 0) { base[p] = -1;} 
    }
    STL_clear(list);
}

template<class T>
void DoubleArrayTrie::STL_clear(T& obj){
    T empty;
    empty.swap(obj);
}


void DoubleArrayTrie::print(){
    for (int i=0; i<20 ; i++){
        wcout << i;
        wcout << " ";
        wcout << base[i];
        wcout << " ";
        wcout << check[i];
        wcout << " " << endl;
    }
    wcout << "-------------" << endl;
}

int DoubleArrayTrie::get_parent_state(wstring seg){
    wstring word = wstring(1, seg[0]);
    int code = vocab[word];
    int p = code;
    int b = 0;
    for(int i=1; i<seg.size(); i++){
        word = wstring(1, seg[i]);
        code = vocab[word];
        p = abs(base[p]) + code;
    }
    return p;
}

void DoubleArrayTrie::fetch_siblings(vector<wstring> &segments, deque<vector<Node>>& queue){
    siblings_def siblings;
    for (int i=0; i<segments.size(); i++){
        wstring seg = segments[i];
        wstring pword = wstring(1, seg[0]);
        vocab[pword] = vocab[pword] == 0 ? ++num_words : vocab[pword];
        int pcode = vocab[pword];
        check[pcode] = 1;

        if (seg.size() == 1) { continue; }
        Node node;
        wstring word = wstring(1, seg[1]);
        vocab[word] = vocab[word] == 0 ? ++num_words : vocab[word];
        node.code = vocab[word];
        node.word = word;
        node.col = 1;
        node.seg = seg;

        siblings[pword].push_back(node);
    }
    for (auto it=siblings.begin(); it!=siblings.end(); it++){
        queue.push_back(it->second);
    }
}

void DoubleArrayTrie::fetch_siblings(vector<Node>& siblings, deque<vector<Node>>& queue){
    siblings_def siblings_map;
    for (Node pnode: siblings){
        wstring seg = pnode.seg;
        wstring pword = pnode.word;
        int pcol = pnode.col;
        if (pcol >= seg.size() - 1) { continue; }

        Node node;
        node.seg = seg;
        node.col = pcol + 1;
        wstring word = wstring(1, seg[node.col]);

        vocab[word] = vocab[word] == 0 ? ++num_words : vocab[word];
        node.code = vocab[word];
        node.word = word;
        siblings_map[pword].push_back(node);
    }
    for (auto it=siblings_map.begin(); it!=siblings_map.end(); it++){
        queue.push_back(it->second);
    }
}


int DoubleArrayTrie::find_begin(vector<Node>& siblings){
    int pos = siblings[0].code + 1 > max_index ? siblings[0].code + 1: max_index;
    bool is_found = true;
    int begin = 0;
    int first = 0;
    while(true){
        for (Node node: siblings){
            begin = pos - siblings[0].code;
            int code = node.code;
            if (code + begin >= alloc_size) { reallocate_storage(code + begin); throw;}
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

vector<string> DoubleArrayTrie::search(string& to_search){
    wstring to_searchw = string_to_wstring(to_search);
    return search(to_searchw);
}

vector<string> DoubleArrayTrie::search(const wstring& to_searchw){
    vector<int> index;
    int end_index = to_searchw.size();
    for (int i=0; i<to_searchw.size() - 1; i++){
        index = prefix_search(to_searchw.substr(i, end_index));
        for(int j : index) {
            wcout << to_searchw.substr(i, j) << endl;
        }
    }
    return vector<string>(); 
}

vector<string> DoubleArrayTrie::common_prefix_search(string& to_search){
    vector<int> index = prefix_search(to_search);
    wstring seg = string_to_wstring(to_search);
    for (int i: index){ 
        wcout << seg.substr(0, i) << endl;
    }
    STL_clear(index);
    return vector<string> {};
}

vector<int> DoubleArrayTrie::prefix_search(string& to_search){
    wstring to_searchw = string_to_wstring(to_search);
    return prefix_search(to_searchw);
}

vector<int> DoubleArrayTrie::prefix_search(const wstring& seg){
    wstring word = seg.substr(0, 1);
    int code = vocab[word];
    int p = code;
    int b = 0;
    vector<int> index;
    if (code==0 || check[code] != 0) { return index; }
    
    if (seg.size() == 1){
        if (base[code] == 0 && check[code] == 0){
            index.push_back(0);
            return index;
        }
    }
    for(int i=1; i<seg.size(); i++){
        word = seg[i];
        code = vocab[word];

        b = abs(base[p]) + code;
        int bp = base[p];
        if (bp < 0) {
            index.push_back(i);
        }
        if (check[b] == p && code != 0){
            p = b;
            continue;
        }
        return index;
    }
    index.push_back(seg.size());
    return index;
}

void DoubleArrayTrie::loop_map(unordered_map<wstring, int> map){
    for (auto it=map.begin(); it!=map.end(); it++){
        wcout << it->first << ":" << it->second << endl;
    }
}

int main(){
    locale::global(locale(""));
    wcout.imbue(locale(""));
    string to_search = "石家庄凯天商贸有限公司";
    wstring b = string_to_wstring(to_search);

    DoubleArrayTrie dat;
    vector<wstring> company = read_file("test");
    // vector<wstring> company = read_file("/home/ubuntu/SocialCredits/CompanyName/company_names.txt");
    // vector<wstring> company = {L"he" ,L"her", L"his", L"se", L"she", L"hers", L"sers"};
    // vector<wstring> company = {L"n拉伯", L"阿拉伯人", L"阿拉伯"};
    time_t start, stop;
    start = time(NULL);
    dat.make_ac(company);
    stop = time(NULL);
    wcout << "cost: " << stop - start << endl; 
    dat.search(to_search);
    pause();
}


