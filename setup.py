#!/usr/bin/env python  
  
from distutils.core import setup, Extension  
  

# the c++ extension module
extension_mod = Extension("datac.datac", 
                        sources=['datac/src/datac.cpp', 'datac/src/double_array_trie.cpp', 'datac/src/utils.cpp'],
                        include_dirs=['datac/src', 'datac/include'],
                        library_dirs=['datac/include/boost'],
                        runtime_library_dirs=['datac/include/boost'],
                        libraries=['datac/include/lib/boost_python'],
                        extra_compile_args=['-std=c++11'])

setup(name='datac',
      version='0.1',
      keywords = ("AhoCorasick", "Aho-Corasick", "Entities matching"),  
      description = "Aho-Corasick algorithm C++ version for python",  
      long_description = "Aho-Corasick by Paper:https://pdfs.semanticscholar.org/3547/ac839d02f6efe3f6f76a8289738a22528442.pdf \
                          Reference:https://www.cs.uku.fi/~kilpelai/BSA05/lectures/slides04.pdf	",  
      license = "MIT Licence",  
      url = "https://github.com/yanwii/Aho-Corasick",  
      author = "yanwii",  
      author_email = "yanwii@outlook.com",
      package_dir={'': 'datac'},
      packages=['datac'],
      ext_modules=[extension_mod])