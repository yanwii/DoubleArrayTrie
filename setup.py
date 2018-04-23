#!/usr/bin/env python  
  
from distutils.core import setup, Extension  
  

# the c++ extension module
extension_mod = Extension("pydat.pydat", 
                        sources=['pydat/src/pydat.cpp', 'pydat/src/double_array_trie.cpp', 'pydat/src/utils.cpp'],
                        include_dirs=['pydat/src', 'pydat/include'],
                        library_dirs=['pydat/include/boost'],
                        runtime_library_dirs=['pydat/include/boost'],
                        libraries=['pydat/include/lib/boost_python'],
                        extra_compile_args=['-std=c++11'])

setup(name='pydat',
      version='0.2',
      keywords = ("Double Array Trie", "DAT"),  
      description = "DoubleArrayTrie(DAT) support prefix search & exact search & multiple pattern match for python implemented by c++",  
      long_description = "",  
      license = "MIT Licence",  
      url = "https://github.com/yanwii/DoubleArrayTrie",  
      author = "yanwii",  
      author_email = "yanwii@outlook.com",
      package_dir={'':'pydat'},
      packages=['pydat'],
      ext_modules=[extension_mod])