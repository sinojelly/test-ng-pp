
import sys
import os

from Phase1Result import *

def raw_fatal(line_number, error):
   print >> sys.stderr, line_number, ":", error
   sys.exit(1)

def fatal(line, error):
   raw_fatal(line.get_line_number(), error)

def warning(line, msg):
   print >> sys.stderr, line.get_line_number(), ":", error

def info(line, msg):
   print >> sys.stdout, line.get_line_number(), ":", error

