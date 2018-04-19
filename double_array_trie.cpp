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

cut_seg_def DoubleArrayTrie::cut_seg(vector<string> &list){
    cut_seg_def segments;
    while(!list.empty()){
        string word = list.back();
        list.pop_back();
        try{
            vector<string> cut_seg = cut(word, false);
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
    base = vector<int> (alloc_size, 0);
    check = vector<int> (alloc_size, 0);
    chari = vector<int> (alloc_size, 0);
    failure = vector<int> (alloc_size, 0);
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
    while (col < max_col){
        // Loop the segments then adding word to d while 
        // constructing the base & check.
        siblings_def siblings_map = fetch_siblings(col, list);

        if (col == 0) { col++; continue; }
        if (siblings_map.empty()) { break; }
        int local_max_index = 0;
        for (auto it=siblings_map.begin(); it!=siblings_map.end(); it++){
            wstring parent_word = it->first;
            vector<Node> siblings = it->second;
            start = clock();
            int begin = find_begin(siblings);
            for (int i=0; i<siblings.size(); i++){
                Node node = siblings[i];
                int code = node.code;
                int t = code + begin;
                int s = node.parent_state;
                check[t] = s;
                chari[t] = code;
                // charl[t] = word;
                base[s] = begin;
            }
        }
        col ++;
        // wcout << col << endl;
    }
    // perfct list base
    // if the word is the end of a segment but there are words atfer it(e.g.: he her):
    //  base[i] = -base[i]
    // else
    //  base[i] = -i
    for (int i=0; i<list.size(); i++){
        wstring seg = list[i];
        wchar_t word = seg[0];
        int code = vocab[word];
        check[code] = 0;
        int p = code;
        int b = 0;
        int bp = 0;
        // make failure
        for (int j=1; j<seg.size(); j++){
            word = seg[j];
            code = vocab[word];
            b = abs(base[p]) + code;
            failure[b] = find_failure(word, failure[p]);
            p = b;
        }
        // perfect list base
        bp = base[p];
        if (bp > 0) { base[p] = -base[p];}
        else if (bp == 0) { base[p] = -1;} 
    }
    STL_clear(list);
    print();
}

void DoubleArrayTrie::search(wstring seg){
    vector<int> result;
    for (int i=1; i < seg.size()-1; i++){
        wstring to_search = seg.substr(i, seg.size());
        vector<int> tmp = prefix_search(to_search);
        for (int index:tmp) { wcout << seg.substr(i, index) << endl;}
    }
}

int DoubleArrayTrie::find_failure(const wchar_t& word, int bp){
    int code = vocab[word];
    if (code == 0) { return 0;}
    /*

    */
    int b = bp + code;
    if (chari[b] == code) {
        return abs(base[b]);
    }
    return 0;
}


template<class T>
void DoubleArrayTrie::STL_clear(T& obj){
    T empty;
    empty.swap(obj);
}


void DoubleArrayTrie::print(){
    for (int i=0; i<11; i++){
        wcout << i;
        wcout << " ";
        wcout << base[i];
        wcout << " ";
        wcout << check[i];
        wcout << " ";
        wcout << chari[i];
        wcout << " ";
        wcout << failure[i] << endl;
    }
    wcout << "-------------" << endl;
}

int DoubleArrayTrie::get_parent_state(wstring seg){
    wchar_t word = seg[0];
    int code = vocab[word];
    int p = code;
    int b = 0;
    for(int i=1; i<seg.size(); i++){
        word = seg[i];
        code = vocab[word];
        p = abs(base[p]) + code;
    }
    return p;
}

siblings_def DoubleArrayTrie::fetch_siblings(int col, vector<wstring> &segments){
    // collect siblings and figure out begin
    siblings_def siblings_map;
    wstring parent_seg = L"";
    for(int i=0; i<segments.size(); i++){
        wstring seg = segments[i];
        if (col >= seg.size()) { continue; }
        wchar_t word = seg[col];
        vocab[word] = vocab[word] == 0 ? vocab[word] = ++nums_word : vocab[word];
        int code = vocab[word];

        if (col == 0) { 
            check[code] = 1;
            chari[code] = code;
            continue;
        } else {
            wstring parent_word = to_wstring(seg[col - 1]);
            Node node;
            for (int i=0; i<col; i++) { parent_seg += seg[i];}
            node.code = code;
            node.word = word;
            node.parent_state = get_parent_state(parent_seg);
            siblings_map[parent_word].push_back(node);
            parent_seg = L"";
        }
    }
    return siblings_map;
}

int DoubleArrayTrie::find_begin(vector<Node> siblings){
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

vector<string> DoubleArrayTrie::common_prefix_search(string& to_search){
    vector<int> index = prefix_search(to_search);
    wstring seg = string_to_wstring(to_search);
    for (int i: index){ 
        wcout << seg.substr(0, i) << endl;
    }
    STL_clear(index);
    cout << "done" << endl;
    return vector<string> {};
}

vector<int> DoubleArrayTrie::prefix_search(string& to_search){
    wstring to_search_w = string_to_wstring(to_search);
    return prefix_search(to_search_w);
}

vector<int> DoubleArrayTrie::prefix_search(wstring& seg){
    wchar_t word = seg[0];
    int code = vocab[word];
    int p = code;
    int b = 0;
    vector<int> index;
    string path = to_string(code);
    if (code == 0) { return index; }
    for(int i=1; i<seg.size(); i++){
        word = seg[i];
        code = vocab[word];

        b = abs(base[p]) + code;
        int bp = base[p];
        if (bp < 0 && is_valid(p, path)) {
            index.push_back(i);
        }
        if ((check[b] == p || chari[b] == code )&& code != 0){
            p = b;
            path += "_" + to_string(b);
            continue;
        }
        return index;
    }
    if (is_valid(p, path)){
        index.push_back(seg.size());
    }
    return index;
}

bool DoubleArrayTrie::is_valid(int p, string path){
    string check_path = to_string(p);
    while (check[p] != 0){
        p = check[p];
        check_path = to_string(p) + "_" + check_path;
    }
    if (path == check_path) { return true;}
    else { return false; }
}

void DoubleArrayTrie::loop_map(unordered_map<wchar_t, int> map){
    for (auto it=map.begin(); it!=map.end(); it++){
        wcout << it->first << ":" << it->second << endl;
    }
}

vector<string> read(string file_name){
    ifstream ifs(file_name, ifstream::in);
    string line;
    vector<string> company_names;
    while(getline(ifs, line)){
        company_names.push_back(line);
    }
    return company_names;
}


int main(){
    locale::global(locale(""));
    wcout.imbue(locale(""));
    string to_search = "德福木工刀具（天津）有限公司sss";
    DoubleArrayTrie dat;
    vector<wstring> company = read_file("test");
    // vector<wstring> company = read_file("/home/ubuntu/SocialCredits/CompanyName/company_names.txt");
    // vector<wstring> company = {L"he", L"his", L"she", L"hers"};
    // vector<wstring> company = {L"n拉伯", L"阿拉伯人", L"阿拉伯"};
    time_t start, stop;
    start = time(NULL);
    dat.make_ac(company);
    stop = time(NULL);
    // cout << "cost: " << stop - start << endl; 
    dat.common_prefix_search(to_search);
    dat.search(L"usshi阿拉伯shers");

}


