# tmpsort
Implementation of mergesort in C++ using template metaprogramming. This implementation runs the entirety of the mergesort at compile-time.

This code works with C++11 or above. Tested with GCC 9.3.0, Clang 10.0.0, and MSVC 19.22.27905. To use, simply compile mergesort.cpp and run the resulting executable. Should work on any platform with any compiler.

* tmplist.hpp: a single-header list library. Contains a list template and some utility routines for working with the lists at compile-time.
* mergesort.cpp: the mergesort implementation. Also contains a main function with some tests.
* tests_tmplist.cpp: a small test program to do some sanity checks on tmplist.hpp.

For more information, see the docstrings in the files.

This project is open-sourced under the MIT license. See the LICENSE file for full details.
