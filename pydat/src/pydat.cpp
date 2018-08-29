#include <iostream>
#include <vector>
#include "double_array_trie.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace boost::python;
using namespace std;
namespace bp = boost::python;

#define BP_REGISTER_SHARED_PTR_TO_PYTHON(PTR) do { \
  const boost::python::type_info info = \
    boost::python::type_id<shared_ptr<PTR > >(); \
  const boost::python::converter::registration* reg = \
    boost::python::converter::registry::query(info); \
  if (reg == NULL) { \
    bp::register_ptr_to_python<shared_ptr<PTR > >(); \
  } else if ((*reg).m_to_python == NULL) { \
    bp::register_ptr_to_python<shared_ptr<PTR > >(); \
  } \
} while (0)

vector<string>  (DoubleArrayTrie::*search_entry)(string) = &DoubleArrayTrie::search;
vector<string>  (DoubleArrayTrie::*greedy_search_entry)(string) = &DoubleArrayTrie::greedy_search;
void    (DoubleArrayTrie::*add_word_entry)(string) = &DoubleArrayTrie::add_word;

// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS()
BOOST_PYTHON_MODULE(pydat){
    class_<vector<string>, shared_ptr<vector<string>> >("string_vector")
        .def(vector_indexing_suite<vector<string> >());
    boost::python::register_ptr_to_python<boost::shared_ptr<vector<string> > >();

    class_<DoubleArrayTrie >("DoubleArrayTrie")
        .def("search", search_entry)
        .def("add_word", add_word_entry)
        .def("greedy_search", greedy_search_entry)
        .def("make", &DoubleArrayTrie::make)
        .def("set_alloc", &DoubleArrayTrie::set_alloc)
        .def("common_prefix_search", &DoubleArrayTrie::common_prefix_search)
        .def("load_file", &DoubleArrayTrie::load_file);
    BP_REGISTER_SHARED_PTR_TO_PYTHON(DoubleArrayTrie);
}