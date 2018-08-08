#!/usr/bin/env python  
  
from distutils.core import setup, Extension
import sys
import site

py_version = 2

package_data={"pydat":["libboost_python2.so.1.66.0"]},
if sys.version.split(".")[0] == '2':
      py_version = 2
      package_data = {"pydat":["libboost_python2.so.1.66.0"]}
elif sys.version.split(".")[0] == '3':
      py_version = 3
      package_data={"pydat":["libboost_python3.so.1.66.0"]}

runtime_dirs = [i + "/pydat" for i in site.getsitepackages()]

# the c++ extension module
extension_mod = Extension("pydat.pydat", 
                        sources=['pydat/src/pydat.cpp', 'pydat/src/double_array_trie.cpp', 'pydat/src/utils.cpp'],
                        include_dirs=['pydat/src'],
                        library_dirs=['pydat/pydat'],
                        runtime_library_dirs=runtime_dirs,
                        libraries=['boost_python'],
                        extra_compile_args=['-std=c++11'])

long_des = ""
with open("./README.rst") as fopen
      long_des = fopen.read()

setup(name='pydat',
      version='0.4.3',
      keywords = ("Double Array Trie", "DAT"),  
      description = "DoubleArrayTrie(DAT) support prefix search & exact search & multiple pattern match for python implemented by c++",  
      long_description = long_des,  
      license = "MIT Licence",
      url = "https://github.com/yanwii/DoubleArrayTrie",  
      author = "yanwii",  
      author_email = "yanwii@outlook.com",
      package_dir={'':'pydat'},
      packages=['pydat'],
      package_data=package_data,
      ext_modules=[extension_mod]
)