
import sys
import os

from Phase1Result import *

def raw_fatal(file, line_number, error):
   print >> sys.stderr, file + ":" + str(line_number) + ": error:", error
   sys.exit(1)

def fatal(file, line, error):
   raw_fatal(file, line.get_line_number(), error)

def warning(file, line, msg):
   print >> sys.stderr, line.get_line_number(), ":", error

def info(file, line, msg):
   print >> sys.stdout, line.get_line_number(), ":", error

