#include <iostream>
#include <vector>
#include <deque>
#include "double_array_trie.h"
#include "utils.h"
#include <math.h>
#include <string>
#include <time.h>
#include <locale>
#include <fstream>
using namespace std;

void DoubleArrayTrie::add_word(string seg){
    /* Add word */
    wstring wseg = string_to_wstring(seg);
    num_chars += wseg.size();
    segments.push_back(wseg);
}

void DoubleArrayTrie::add_word(const wstring& wseg){
    /* Add word */
    num_chars += wseg.size();
    segments.push_back(wseg);
}


void DoubleArrayTrie::add_words(vector<string> words){
    /* 
        Add words
        First clear the vector, then convert string to wstring
    */
    STL_clear(segments);
    for (int i=0; i<words.size(); i++){
        wstring wword = string_to_wstring(words[i]);
        segments.push_back(wword);
    }
}

void DoubleArrayTrie::add_words(vector<wstring> wwords){
    STL_clear(segments);
    segments.insert(segments.end(), wwords.begin(), wwords.end());
}

void DoubleArrayTrie::reallocate_storage(int new_size){
    /* Dynamic allocate the storage */
    vector<int> pad(new_size - base.size() + 1);
    base.insert(base.end(), pad.begin(), pad.end());
    check.insert(check.end(), pad.begin(), pad.end());
    alloc_size = new_size + 1;
}

void DoubleArrayTrie::init_storage(){
    /* Initlize all variable */
    STL_clear(base);
    STL_clear(check);
    STL_clear(vocab);
    num_words = 0;
    num_chars = 0;
    max_index = 0;

    base = vector<int> (alloc_size, 0);
    check = vector<int> (alloc_size, 0);
}

void DoubleArrayTrie::set_alloc(int size){
    /* Pre set the allocation size */
    alloc_size = size;
}

void DoubleArrayTrie::make(){
    /* Make the base & check then clear segemnts */
    // storage allocation
    init_storage();

    // first collection
    deque<vector<Node>> queue;
    
    fetch_siblings(queue);
    while (!queue.empty()){
        // loop the node while 
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
    for (int i=0; i<segments.size(); i++){
        wstring seg = segments[i];
        wstring word = wstring(1, seg[0]);
        int code = vocab[word];
        int p = code;
        int b = 0;
        int bp = 0;
        int failure_point = 0;
        for (int j=1; j<seg.size(); j++){
            word = wstring(1, seg[j]);
            code = vocab[word];
            b = abs(base[p]) + code;
            p = b;
            failure_point = find_failure(code, failure_point);
            if ( failure_point != 0 ) { failure[p] = failure_point; }
        }
        bp = base[p];
        if (bp > 0) { base[p] = -base[p]; }
        else if (bp == 0) { base[p] = -1; }
        output[p] = i;
    }
    // STL_clear(segments);
}

template<class T>
void DoubleArrayTrie::STL_clear(T& obj){
    T empty;
    empty.swap(obj);
}

int DoubleArrayTrie::find_failure(int code, int parent_failure){
    /* To find out the failure path of the node */
    int failure = 0;
    if (parent_failure == 0){
        if (check[code] == -1){
            failure = code;
        } 
    } else {
        int b = abs(base[parent_failure]) + code;
        if (check[b] == parent_failure) {
            failure = b;
        }
    }
    return failure;
}

int DoubleArrayTrie::get_parent_state(const wstring& seg){
    wstring word = wstring(1, seg[0]);
    int code = vocab[word];
    int p = code;
    for(int i=1; i<seg.size(); i++){
        word = wstring(1, seg[i]);
        code = vocab[word];
        p = abs(base[p]) + code;
    }
    return p;
}

void DoubleArrayTrie::fetch_siblings(deque<vector<Node>>& queue){
    siblings_def siblings;
    for (int i=0; i<segments.size(); i++){
        wstring seg = segments[i];
        wstring pword = wstring(1, seg[0]);
        vocab[pword] = vocab[pword] == 0 ? ++num_words : vocab[pword];
        int pcode = vocab[pword];
        // reallocate storage
        if (pcode >= alloc_size) { reallocate_storage(pcode); }
        check[pcode] = -1;

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
    /*
        Find a begin satisfies:
        check[begin + s[i]] = base[begin + s[i]] == 0
        where s[i] stands for all siblings
    */
    int pos = siblings[0].code + 1 > max_index ? siblings[0].code + 1: max_index;
    bool is_found = true;
    int begin = 0;
    int first = 0;
    while(true){
        for (Node node: siblings){
            begin = pos - siblings[0].code;
            int code = node.code;
            if (code + begin >= alloc_size) { reallocate_storage(code + begin); }
            if (base[code + begin] !=0 || check[code + begin] !=0){
                is_found = false;
                break;
            }
        }
        if (!is_found){
            max_index = pos;
            pos++;
            is_found = true;
        } else {
            return begin;
        }
    }

}

vector<string> DoubleArrayTrie::common_prefix_search(string to_search){
    /* Perfix search entry*/
    vector<int> index = prefix_search(to_search);
    wstring seg = string_to_wstring(to_search);
    vector<string> match_result;
    for (int i: index){ 
        string index_s = to_string(0) + "_" + to_string(i);
        match_result.push_back(index_s);
    }
    return match_result;
}

vector<int> DoubleArrayTrie::prefix_search(string& to_search){
    /* Perfix search */
    wstring to_searchw = string_to_wstring(to_search);
    return prefix_search(to_searchw);
}

vector<int> DoubleArrayTrie::prefix_search(const wstring& seg){
    /* Perfix search */
    wstring word = seg.substr(0, 1);
    int code = vocab[word];
    int p = code;
    int b = 0;
    vector<int> index;
    if (code==0 || check[code] != 0) { return index; }
    
    if (seg.size() == 1){
        if (base[code] == -1 && check[code] == -1){
            index.push_back(1);
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
    int bp = base[p];
    if (bp < 0){ index.push_back(seg.size()); }
    return index;
}

vector<string> DoubleArrayTrie::search(string to_search){
    wstring to_searchw = string_to_wstring(to_search);
    return search(to_searchw);
}

void DoubleArrayTrie::check_output(int p, int end, vector<string>& index){
    if ( base[p] < 0 ){
        wstring out = segments[output[p]];
        int begin = end - out.size();
        index.push_back(to_string(begin) + "_" + to_string(end));
        // cout << "out " <<  wstring_to_string(segments[output[p]]) << "     " << index[index.size() - 1] << endl;
    }
}

vector<string> DoubleArrayTrie::search(const wstring& to_searchw){
    wstring word = L"";
    vector<string> index;
    int b = 0;
    int p = 0;
    int code = 0;
    for (int i=0; i<to_searchw.size(); i++){
        word = to_searchw[i];
        code = vocab[word];
        b = abs(base[p]) + code;

        if ( code == 0 ){
            check_output(p, i, index);
            p = 0;
        } else if ( check[b] == p ){
            p = b;
        } else if ( check[b] == -1 && p == 0){
            p = b;
        } else if ( failure[p] != 0 ){
            check_output(p, i, index);
            p = failure[p];
            i --;
        } else if ( p == 0 && check[b] != -1){
            check_output(p, i, index);
            p = 0;
        } else {
            check_output(p, i, index);
            p = 0;
            i --;
        }
    }
    check_output(p, to_searchw.size(), index);
    return index;
}

vector<string> DoubleArrayTrie::maximum_search(string to_search){
    wstring to_searchw = string_to_wstring(to_search);
    return maximum_search(to_searchw);
}

vector<string> DoubleArrayTrie::maximum_search(const wstring& to_searchw){
    wstring word = L"";
    vector<string> index;
    int b = 0;
    int p = 0;
    int code = 0;
    for (int i=0; i<to_searchw.size(); i++){
        word = to_searchw[i];
        code = vocab[word];
        b = abs(base[p]) + code;
        if ( code == 0){
            p = 0;
        } else if ( check[b] == p ){
            p = b;
        } else if ( check[b] == -1 && p == 0 ){
            p = b;
        } else if ( failure[p] != 0){
            p = failure[p];
            i --;
        } else if ( p == 0 && check[b] != -1){
            p = 0;
        } else {
            p = 0;
            i --;
        }
        check_output(p, i + 1, index);
    }
    return index;
}

void DoubleArrayTrie::load_file(const string& file_name){
    /* Load segments from local file */
    segments = read_file(file_name);
}

int main(){
    DoubleArrayTrie dat;
    // dat.load_file("test.txt");
    dat.load_file("/home/ubuntu/SocialCredits/CompanyName/short");
    dat.add_word("派特尔");
    dat.add_word("海天味业");

    string to_search = "";
    dat.make();
    cout << dat.segments.size() << endl;
    for(wstring seg: dat.segments){
        to_search += wstring_to_string(seg);
    }

    // to_search = "派特尔海天味业";
    vector<string> index_s = dat.search(to_search);
    cout << index_s.size() << endl;
    // for (string i:index_s){ cout << i << endl; }
    cout << "----------------" << endl;
    index_s = dat.maximum_search(to_search);
    cout << index_s.size() << endl;
    // for (string i:index_s){ cout << i << endl; }
}
