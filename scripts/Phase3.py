#!/usr/bin/python

import re
import codecs

from Phase1Result import *
from PreprocessScopeParser import *

from FixtureParser import FixtureParser

fixture_re1  = re.compile( r'''^\s*FIXTURE\s*\(\s*(?P<fixtureId>[A-Za-z_][A-Za-z0-9_]*)\s*(,\s*(?P<fixtureName>\w+))?\)\s*''', re.UNICODE)

fixture_re2  = re.compile( r'''^\s*(struct|class)\s+(?P<fixtureId>[A-Za-z_][A-Za-z0-9_]*)\s*:\s*public\s+((TESTNGPP_NS|TESTCPP_NS|testngpp|testcpp)\s*::)?\s*TestFixture\s*$''', re.UNICODE)

##########################################################
def is_fixture_def(line):
   matched = fixture_re1.match(line.get_content())
   if matched:
      return matched.group("fixtureId"), matched.group("fixtureName")

   matched = fixture_re2.match(line.get_content())
   if matched:
      return matched.group("fixtureId"), None

   return None
   
##########################################################
def phase3(globalScope, file):
   return PreprocessScopeParser(file, globalScope, is_fixture_def, FixtureParser, "fixture").parse()

