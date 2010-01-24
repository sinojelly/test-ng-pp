#!/usr/bin/python

import sys

from TestScope import TestScope

###########################################
class Fixture:
   ########################################
   def __init__(self, name, file, line):
      self.id   = name[0]
      self.name = name[1]
      self.scope = TestScope("::", None)
      self.file = file
      self.line = line

   def show(self):
      content = "FIXTURE("+self.get_name()+")"
      print content.encode('utf-8')
      self.scope.show()

   ########################################
   def get_id(self):
      return self.id

   ########################################
   def get_name(self):
      if self.name:
         return self.name

      return self.id

   ########################################
   def add_elem(self, testcase):
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
