#!/usr/bin/python

import re
import codecs

from Phase1Result import *
from PreprocessScopeParser import *
from ElemParser import ElemParser

from FixtureParser import FixtureParser

from Message import *

fixture_re1  = re.compile( \
   r'''^\s*FIXTURE\s*\(\s*(?P<fixtureId>[A-Za-z_][A-Za-z0-9_]*)\s*(,\s*(?P<fixtureName>\w+))?\)\s*''', \
   re.UNICODE)

fixture_re2  = re.compile( \
   r'''^\s*(struct|class)\s+(?P<fixtureId>[A-Za-z_][A-Za-z0-9_]*)\s*:\s*public\s+((TESTNGPP_NS|TESTCPP_NS|testngpp|testcpp)\s*::)?\s*TestFixture\s*$''' )

fixture_re3  = re.compile( \
   r'''^\s*(struct|class)\s+(?P<fixtureId>[A-Za-z_][A-Za-z0-9_]*)\s*:\s*public\s+((CxxTest)\s*::)?\s*TestSuite\s*$''' )
##########################################################
def is_fixture_def(line):
   matched = fixture_re1.match(line)
   if matched:
      return matched.group("fixtureId"), matched.group("fixtureName")

   matched = fixture_re2.match(line)
   if matched:
      return matched.group("fixtureId"), None

   matched = fixture_re3.match(line)
   if matched:
      return matched.group("fixtureId"), None

   return None
   
##########################################################
class GlobalParser:
   def __init__(self, file):
      self.file = file

   def is_elem_def(self, content):
      return is_fixture_def(content)

   def handle_tag(self, tag):
      warning(self.file, tag, "useless tag definition @" + tag.get_tag())

   def verify_scope(self, tag):
      return True

   def create_elem_parser(self, elem_name, file, line):
      return FixtureParser(elem_name, file, line)

   def get_type_name(self):
      return "fixture"

   def parse_line(self, line):
      return None

   def get_container(self):
      return None

##########################################################
def phase3(globalScope, file):
   return PreprocessScopeParser(file, globalScope, GlobalParser(file), ElemParser).parse()

