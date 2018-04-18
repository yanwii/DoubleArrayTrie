#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <fstream>
#include <functional>
#include <string>
#include "utils.h"
using namespace std;

vector<string> cut(const string &word, bool if_reverse=false){
    vector<string> segments;
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

vector<string> read_file(string file_name){
    ifstream ifs(file_name, ifstream::in);
    string line;
    vector<string> company_names;
    while(getline(ifs, line)){
        company_names.push_back(line);
    }
    ifs.close();
    return company_names;
}


wstring string_to_wstring(const string& s)
{
    std::string strLocale = setlocale(LC_ALL, "");
    const char* chSrc = s.c_str();
    size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
    wchar_t* wchDest = new wchar_t[nDestSize];
    wmemset(wchDest, 0, nDestSize);
    mbstowcs(wchDest,chSrc,nDestSize);
    std::wstring wstrResult = wchDest;
    delete []wchDest;
    setlocale(LC_ALL, strLocale.c_str());
    return wstrResult;
}