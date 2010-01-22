#!/usr/bin/python

import sys
import re
import os
import codecs

from Phase1Result import *
from Phase2Result import *
from Phase3Result import *

from FixtureParser import FixtureParser

from Message import *


fixture_re  = re.compile( r'^\s*FIXTURE\s*\(\s*(?P<fixtureName>\w+)\s*\)\s*', re.UNICODE)

##########################################################
def is_fixture_def(line):
   m = fixture_re.match(line.get_content())
   if m:
      return m.group("fixtureName")

   return None
   
##########################################################
class FixturesInOneFile(Fixtures):
   #######################################################
   def __init__(self, inst, expr):
      Fixtures.__init__(self, inst, expr)

   #######################################################
   def add_fixture(self, fixture):
      self.fixtures.append(fixture)

   #######################################################
   def get_current_fixture(self):
      return self.fixtures[-1]

   #######################################################
   def add_scope(self, scopes):
      self.scopes.append(scopes)

   #######################################################
   def add_sibling(self, sibling):
      self.siblings.append(sibling)


##########################################################
class Phase3Parser:
   #######################################################
   def __init__(self, scope):
      self.scope = scope
      self.scopes = []

      self.fixtures = FixturesInOneFile(scope.get_name(), scope.get_expr())
      self.parser = None

   #######################################################
   def show(self):
      self.fixtures.show()

   #######################################################
   def handle_tag(self, tag):
      pass

   #######################################################
   def look_for_fixture(self, line):
      if isinstance(line, Tag):
         self.handle_tag(line)
         return

      if isinstance(line, Scope):
         self.scopes.append(line)
         return

      fixture_name = is_fixture_def(line)
      if fixture_name:
         self.parser = FixtureParser(fixture_name)

   #######################################################
   def parse_line(self, line):
      if self.parser:
         fixture = self.parser.parse(line)
         if fixture:
            self.fixtures.add_fixture(fixture)
            self.parser = None
         return

      self.look_for_fixture(line)      
      
   #######################################################
   def parse(self):
      for line in self.scope.get_lines():
         self.parse_line(line)

      if self.parser:
         fatal(self.scope.get_lines()[-1], "incompleted fixture definition")

      # sub scope
      for scope in self.scopes:
         self.fixtures.add_scope(Phase3Parser(scope).parse())

      for scope in self.scope.get_elses():
         self.fixtures.add_sibling(Phase3Parser(scope).parse())
         
      return self.fixtures

##########################################################
def phase3(globalScope):
   ''' return: Fixtures'''
   return Phase3Parser(globalScope).parse()

