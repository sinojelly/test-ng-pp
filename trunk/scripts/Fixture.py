#!/usr/bin/python

import sys

from TestScope import TestScope

###########################################
class Fixture:
   ########################################
   def __init__(self, name, file, line):
      self.name = name
      self.scope = TestScope("::", None)
      self.file = file
      self.line = line

   def show(self):
      content = "FIXTURE("+self.name+")"
      print content.encode('utf-8')
      self.scope.show()

   ########################################
   def get_name(self):
      return self.name

   ########################################
   def add_testcase(self, testcase):
      self.scope.add_elem(testcase)

   ########################################
   def add_sub_scope(self, scope):
      self.scope.add_sub_scope(scope)

   ########################################
   def get_scope(self):
      return self.scope

   ########################################
   def get_file_name(self):
      return self.file

   ########################################
   def get_line_number(self):
      return self.line

###########################################
