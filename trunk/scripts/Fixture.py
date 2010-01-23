#!/usr/bin/python

from TestScope import TestScope

###########################################
class Fixture:
   ########################################
   def __init__(self, name):
      self.name = name
      self.scope = TestScope("::", None)

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

###########################################
