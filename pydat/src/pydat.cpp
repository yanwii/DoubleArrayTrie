#include <iostream>
#include <vector>
#include "double_array_trie.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace boost::python;
using namespace std;

vector<string>  (DoubleArrayTrie::*search_entry)(string) = &DoubleArrayTrie::search;
void    (DoubleArrayTrie::*add_word_entry)(string) = &DoubleArrayTrie::add_word;

// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS()
BOOST_PYTHON_MODULE(pydat){
    class_<vector<string> >("string_vector")
        .def(vector_indexing_suite<vector<string> >());

    class_<DoubleArrayTrie>("DoubleArrayTrie")
        .def("search", search_entry)
        .def("add_word", add_word_entry)
        .def("make", &DoubleArrayTrie::make)
        .def("set_alloc", &DoubleArrayTrie::set_alloc)
        .def("common_prefix_search", &DoubleArrayTrie::common_prefix_search)
        .def("load_file", &DoubleArrayTrie::load_file);
}