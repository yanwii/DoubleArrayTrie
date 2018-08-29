#!/usr/bin/env python  
  
from distutils.core import setup, Extension
import sys
import site
import shutil

py_version = 2
tag = sys.argv[1]

def copyfile(srcfile, file_l):
      if tag == "sdist":
            return
      for tfile in file_l:
            if tfile == srcfile:
                  continue
            shutil.copyfile("pydat/pydat/{}".format(srcfile), "pydat/pydat/{}".format(tfile))

package_data = ["libboost_python2.so.1.66.0", "libboost_python3.so.1.66.0"]
if sys.version.split(".")[0] == '2':
      py_version = 2
      ext_package_data = ["libboost_python.so.1.66.0", "libboost_python-2.so.1.66.0", "libboost_python2.so"]
      package_data = {"pydat":ext_package_data + package_data}
      copyfile("libboost_python2.so.1.66.0", ext_package_data)
      libraries = ['boost_python2']
elif sys.version.split(".")[0] == '3':
      py_version = 3
      ext_package_data = ["libboost_python.so.1.66.0", "libboost_python-3.so.1.66.0", "libboost_python3.so"]
      package_data = {"pydat":package_data + ext_package_data }
      copyfile("libboost_python3.so.1.66.0", ext_package_data)
      libraries = ['boost_python3']

runtime_dirs = [i + "/pydat" for i in site.getsitepackages()]

# the c++ extension module
extension_mod = Extension("pydat.pydat", 
                        sources=['pydat/src/pydat.cpp', 'pydat/src/double_array_trie.cpp', 'pydat/src/utils.cpp'],
                        include_dirs=['pydat/src', "pydat/include"],
                        library_dirs=['pydat/pydat'],
                        runtime_library_dirs=runtime_dirs,
                        libraries=libraries,
                        extra_compile_args=['-std=c++11'])

long_des = ""
try:
      with open("./README.rst") as fopen:
            long_des = fopen.read()
except Exception:
      pass

setup(name='pydat',
      version='0.6',
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