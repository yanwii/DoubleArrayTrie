#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <fstream>
#include <functional>
#include "utils.h"
using namespace std;

deque<string> cut(const string &word, bool if_reverse=false){
    deque<string> segments;
    string tmp = "";
    for (int i=0; i < word.size(); i++){
        if(word.at(i) <= -1){
            tmp = "";
            tmp += word.at(i);
            i++;
            tmp += word.at(i);
            i++;
            tmp += word.at(i);
            segments.push_back(tmp);
        } else {
            tmp = word.at(i);
            segments.push_back(tmp);
        }
    }
    if (if_reverse){
        reverse(segments.begin(), segments.end());
    }
    return segments;
}

deque<string> read_file(string file_name){
    ifstream ifs(file_name, ifstream::in);
    string line;
    deque<string> company_names;
    while(getline(ifs, line)){
        company_names.push_back(line);
    }
    ifs.close();
    return company_names;
}

template<class T>
void STLClearObject(T* obj){
    T tmp;
    tmp.swap(*obj);
    // Sometimes "T tmp" allocates objects with memory (arena implementation?).
    // Hence using additional reserve(0) even if it doesn't always work.
}