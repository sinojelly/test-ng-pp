
import os
import sys

##########################################################
class TestCase:
   def __init__(self, name):
      self.name = name
      self.file = "a.h"
      self.line = 100

   def get_name(self):
      return self.name

   def get_file_name(self):
      return self.file

   def get_line_number(self):
      return self.line

##########################################################
class Fixture:
   def __init__(self, name):
      self.name = name
      self.testcases = []

   def get_name(self):
      return self.name

   def get_testcases(self):
      return self.testcases

##########################################################
class Fixtures:
   def __init__(self, inst, expr):
      self.inst = inst
      self.expr = expr
      self.fixtures = []
      self.scopes = []
      self.siblings = []

   def get_inst(self):
      result = self.inst
      if self.expr:
         result = result + " " + self.inst

      return result

   def get_fixtures(self):
      return self.fixtures

   def get_sub_scopes(self):
      return self.scopes
   
   def get_siblings(self):
      return self.siblings

   def is_root_scope(self):
      return self.inst == "ifdef" or \
             self.inst == "ifndef" or \
             self.inst == "if"
