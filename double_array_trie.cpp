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
    vector<Node> siblings = fetch_siblings(list);
    vector<vector<Node>> queue;
    queue.push_back(siblings);
    STL_clear(siblings);

    while (!queue.empty()){
        // Loop the segments then adding word to d while 
        // constructing the base & check.
        siblings = queue.back();
        queue.pop_back();
        fetch_siblings(siblings, queue);
        int begin = find_begin(siblings);
        for (Node node:siblings){
            int code = node.code;
            int t = begin + code;
            int s = node.parent_state;
            check[t] = s;
            chari[t] = code;
            base[s] = t;
        }
    }
    // perfct list base
    // if the word is the end of a segment but there are words atfer it(e.g.: he her):
    //  base[i] = -base[i]
    // else
    //  base[i] = -i
    for (int i=0; i<list.size(); i++){
        wstring seg = list[i];
        wstring word = seg.substr(0, 1);
        int code = vocab[word];
        int p = code;
        int b = 0;
        int bp = 0;
        for (int j=1; j<seg.size(); j++){
            word = seg[j];
            code = vocab[word];
            b = abs(base[p]) + code;
            p = b;
        }
        bp = base[p];
        if (bp > 0) { base[p] = -base[p];}
        else if (bp == 0) { base[p] = -1;} 
    }
    STL_clear(list);
    // print();
}

template<class T>
void DoubleArrayTrie::STL_clear(T& obj){
    T empty;
    empty.swap(obj);
}


void DoubleArrayTrie::print(){
    for (int i=0; i<20 ; i++){
        cout << i;
        cout << " ";
        cout << base[i];
        cout << " ";
        cout << check[i];
        cout << " ";
        cout << chari[i] << endl;
    }
    cout << "-------------" << endl;
}

int DoubleArrayTrie::get_parent_state(wstring seg){
    wstring word = seg.substr(0, 1);
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

vector<Node> DoubleArrayTrie::fetch_siblings(vector<wstring> &segments){
    // collect siblings and figure out begin
    vector<Node> siblings;
    for (int i=0; i<segments.size(); i++){
        wstring seg = segments[i];
        wstring word = seg.substr(0, 1);
        vocab[word] = vocab[word] == 0 ? vocab[word] = ++nums_word : vocab[word];
        Node node;
        node.word = word;
        node.seg = seg;
        node.code = vocab[word];
        node.col = 1;
        siblings.push_back(node);
    }
}

void DoubleArrayTrie::fetch_siblings(vector<Node>&, vector<vector<Node>>& queue){

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
    wstring seg = string_to_wstring(to_search);
    wstring word = seg.substr(0, 1);
    int code = vocab[word];
    
    int p = code;
    int b = 0;
    vector<int> index;
    wstring result = seg.substr(0, 1);
    for(int i=1; i<seg.size(); i++){
        word = seg[i];
        code = vocab[word];

        b = abs(base[p]) + code;
        int bp = base[p];
        if (bp < 0 && seg.substr(0, i) == result) {
            index.push_back(i);
        }
        if ((check[b] == p || chari[b] == code )&& code != 0){
            p = b;
            result += word;
            continue;
        }
        return index;
    }
    index.push_back(seg.size());
    return index;
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
    string to_search = "阿拉伯人";
    wstring b = string_to_wstring(to_search);
    DoubleArrayTrie dat;
    // vector<wstring> company = read_file("test");
    // vector<wstring> company = read_file("/home/ubuntu/SocialCredits/CompanyName/company_names.txt");
    // vector<wstring> company = {L"he" ,L"her", L"his", L"se", L"she", L"hers", L"sers"};
    vector<wstring> company = {L"n拉伯", L"阿拉伯人", L"阿拉伯"};
    time_t start, stop;
    start = time(NULL);
    dat.make_ac(company);
    stop = time(NULL);
    // cout << "cost: " << stop - start << endl; 
    // dat.prefix_search(to_search);
    dat.common_prefix_search(to_search);

}


