#!/usr/bin/python

import re
import codecs

from Phase1Result import *
from PreprocessScopeParser import *

from FixtureParser import FixtureParser

fixture_re  = re.compile( r'^\s*FIXTURE\s*\(\s*(?P<fixtureName>\w+)\s*\)\s*', re.UNICODE)

##########################################################
def is_fixture_def(line):
   m = fixture_re.match(line.get_content())
   if m:
      return m.group("fixtureName")

   return None
   
##########################################################
def phase3(globalScope):
   return PreprocessScopeParser(globalScope, is_fixture_def, FixtureParser, "fixture").parse()

