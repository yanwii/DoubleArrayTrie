#ifndef TTA_UTILS
#define TTA_UTILS

#include <iostream>
#include <vector>
using namespace std;

vector<string> cut(const string&, bool);
vector<wstring> read_file(const string&);
wstring string_to_wstring(const string &);
string wstring_to_string(const wstring &);
#endif