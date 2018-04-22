#include <iostream>
#include "double_array_trie.h"
// #include "../include/boost/python.hpp"
// #include "../include/boost/python/suite/indexing/vector_indexing_suite.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


using namespace boost::python;
using namespace std;

// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS()
BOOST_PYTHON_MODULE(datac){
    class_<DoubleArrayTrie>("DoubleArrayTrie")
        .def_readwrite("num_words", &DoubleArrayTrie::num_words);
}