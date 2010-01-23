
import sys
import os

from Phase1Result import *

def fatal(line, error):
   print >> sys.stderr, line.get_line_number(), ":", error
   sys.exit(1)

def warning(line, msg):
   print >> sys.stderr, line.get_line_number(), ":", error

def info(line, msg):
   print >> sys.stdout, line.get_line_number(), ":", error

def output(content, file):
   print content.encode('utf-8')
