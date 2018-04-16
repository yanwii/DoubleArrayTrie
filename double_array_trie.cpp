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
            deque<string> cut_seg = cut(word, true);
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
    charl = deque<string> (alloc_size, "");
}

void DoubleArrayTrie::make_ac(deque<string> list){
    // storage allocation
    init_storage();

    // deque<string> list = {"he" ,"his", "she", "her", "hers"};
    cut_seg_def segments = cut_seg(list);
    deque<deque<Node>> queue;
    siblings_def sibling_map = fetch_siblings(0, segments);
    for (auto it=sibling_map.begin(); it!=sibling_map.end(); it++){
        queue.push_back(it->second);        
    }
    int col = 0;
    int max_col = 20;
    clock_t start, stop;
    while (!queue.empty()){
        deque<Node> siblings = queue.front();
        queue.pop_front();
        // Loop the segments then adding word to vocab while 
        // constructing the base & check.
        int begin_s = 0;
        siblings_def siblings_map = fetch_siblings(siblings, begin_s);

        for(auto it=siblings_map.begin(); it!=siblings_map.end(); it++){
            string parent_word = it->first;
            siblings = it->second;
            // find begin
            start = clock();
            int begin = find_begin(siblings, 1);
            cout << "find begin cost: " << clock() - start; 
            cout << " begin: " << begin;
            cout << " begin s: " << begin_s;
            cout << " max index" << max_index << endl;
            // collect siblings
            for (Node node: siblings){
                string word = node.word;
                int s = node.parent_state;
                int code = node.code;

                int t = code + begin;
                max_index = t > max_index ? t : max_index;
                check[t] = s;
                charl[t] = word;
                base[s] = begin;
            }
            queue.push_back(siblings);
        }
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
}

void DoubleArrayTrie::print(){
    for (int i=0; i<35 ; i++){
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


siblings_def DoubleArrayTrie::fetch_siblings(int col, cut_seg_def segments){
    siblings_def siblings;
    string parent_word = "root";
    for (deque<string> seg: segments){
        string word = seg[0];
        vocab[word] = vocab[word] == 0 ? ++nums_word : vocab[word];

        check[vocab[word]] = 1;
        charl[vocab[word]] = word;

        Node node;
        node.word = seg[0];
        node.parent_state = 0;
        node.parent_seg = "";
        node.col = 0;
        node.segments = seg;
        node.code = vocab[word];
        siblings[parent_word].push_back(node);
    }
    return siblings;
}

siblings_def DoubleArrayTrie::fetch_siblings(deque<Node> siblings, int& max_s){
    // ignore dup key
    // collect siblings and figure out begin
    siblings_def child_map;
    for (Node node: siblings){
        if ((node.col + 1) < node.segments.size()){
            string parent_word = node.word;
            int col = node.col + 1;
            deque<string> segments = node.segments;
            string word = segments[col];
            vocab[word] = vocab[word] == 0 ? ++nums_word : vocab[word];

            Node child_node;
            child_node.word = word;
            child_node.code = vocab[word];
            child_node.segments = segments;
            child_node.col = col;
            string parent_seg = "";
            for (int i=0; i<col; i++) { parent_seg += segments[i]; }
            child_node.parent_seg = parent_seg;
            int s = get_parent_state(parent_seg);
            child_node.parent_state = s;
            max_s = s > max_s ? s : max_s;
            child_map[parent_word].push_back(child_node);
        }
    }
    return child_map;
}

int DoubleArrayTrie::find_begin(deque<Node> siblings, int begin_s){
    int pos = siblings[0].code + 1 > max_index ? siblings[0].code : max_index - 1; 
    bool is_found = true;
    int begin = 0;
    int first = 0;
    while(true){
        for (Node node: siblings){
            pos ++;
            begin = pos - siblings[0].code;
            int code = node.code;
            if (code + begin > alloc_size) { cout << "leak" << endl;}
            if (base[code + begin] !=0 || check[code + begin] !=0){
                is_found = false;
                break;
            } else if (first == 0){
                max_index = pos;
                first = 1;
            }
        }
        if (!is_found){
            begin ++;
            is_found = true;
        } else {
            return begin;
        }
    }

}

int DoubleArrayTrie::prefix_search(string to_search){
    deque<string> to_searchl = cut(to_search, true);
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
        cout << word << " " << bp << " " << check[b] << " " << p << endl;
        if (bp < 0) { 
            cout << result << endl;
        }
        if (check[b] != p){
            return 0;
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
    deque<string> company = read_file("test");
    // deque<string> company = {"he" ,"her", "his", "se", "she", "hers"};
    // deque<string> company = {"哈哈哈哈", "哈哈哈", "阿胶", "阿根廷", "阿拉伯", "阿拉伯人"};
    time_t start, stop;
    start = time(NULL);
    dat.make_ac(company);
    stop = time(NULL);
    cout << "cost: " << stop - start << endl; 
    dat.prefix_search("沈阳赞星保健品有限公司");
}

