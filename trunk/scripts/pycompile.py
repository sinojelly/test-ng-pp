#!/usr/bin/python

import sys
import py_compile
import compileall

if __name__ == '__main__':
   path = sys.argv[1]
   py_compile.compile(path + '/testngppgen.py')
   compileall.compile_dir(path + '/testngppgen', 0)
