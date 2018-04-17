#ifndef TTA_UTILS
#define TTA_UTILS

#include <iostream>
#include <deque>
using namespace std;

deque<string> cut(const string &word, bool);
deque<string> read_file(string);

template<class T>
void STLClearObject(T*);
#endif