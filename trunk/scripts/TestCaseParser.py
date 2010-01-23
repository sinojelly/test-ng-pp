#!/usr/bin/python

from Phase1Result import *
from TestCase import *
from PreprocessScope import *

from Message import *


##########################################################
class TestCaseParser:
   #######################################################
   def __init__(self, name):
      self.testcase = TestCase(name)
      self.begin = True
      self.done = None
      self.numberOfUnclosedBraces = 0

   #######################################################
   def handle_space(self, line, c):
      if c.isspace():
         return True

      return None
      
   #######################################################
   def handle_done(self, line, c):
      if not self.done:
         return None

      fatal(line, "unexpected char '" + c + "'")
      
   #######################################################
   def handle_begin(self, line, c):
      if not self.begin:
         return None

      if c == '{':
         self.numberOfUnclosedBraces = 1
         self.begin = None
         return True

      fatal(line, "expects '{'")
  
   #######################################################
   def handle_others(self, line, c):
      if c == '{':
         self.numberOfUnclosedBraces += 1
      elif c == '}':
         self.numberOfUnclosedBraces -= 1
         if self.numberOfUnclosedBraces == 0:
            self.done = True
      
   #######################################################
   def handle_char(self, line, c):
      self.handle_space(line, c) or \
      self.handle_done(line, c) or \
      self.handle_begin(line, c) or \
      self.handle_others(line, c)

   #######################################################
   def handle_line(self, line):
      for c in line.get_content():
         self.handle_char(line, c)

      if self.done:
         return self.testcase

      return None

   #######################################################
   def parse(self, line):
      if isinstance(line, Tag):
         # TODO: Warning Here...
         return None
      
      if isinstance(line, PreprocessScope):
         # We don't care scopes in testcases
         return None

      return self.handle_line(line)

