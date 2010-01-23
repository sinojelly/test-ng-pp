#!/usr/bin/python

import re
import codecs

from Phase1Result import *
from PreprocessScopeParser import *

from FixtureParser import FixtureParser

fixture_re  = re.compile( r'''^\s*FIXTURE\s*\(\s*(?P<fixtureId>[A-Za-z_][A-Za-z0-9_]*)\s*(,\s*(?P<fixtureName>\w+))?\)\s*''', re.UNICODE)

##########################################################
def is_fixture_def(line):
   matched = fixture_re.match(line.get_content())
   if matched:
      return matched.group("fixtureId"), matched.group("fixtureName")

   return None
   
##########################################################
def phase3(globalScope):
   return PreprocessScopeParser(globalScope, is_fixture_def, FixtureParser, "fixture").parse()

