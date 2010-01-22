
import os
import sys

##########################################################
class TestCase:
   def __init__(self, name):
      self.name = name

   def get_name(self):
      return self.name

   def show(self):
      print "TEST(", self.name, ")"

##########################################################
class Fixture:
   def __init__(self, name):
      self.name = name
      self.testcases = []

   def get_name(self):
      return self.name

   def get_testcases(self):
      return self.testcases

   def show(self):
      print "FIXTURE(", self.name, ")"
      for testcase in self.testcases:
         testcase.show()

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

   

