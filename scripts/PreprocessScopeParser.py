#!/usr/bin/python

from Phase1Result import *

from TestScope import TestScope
from ElemParser import ElemParser

##########################################################
class PreprocessScopeParser:
   #######################################################
   def __init__(self, file, pp_scope, parser):
      self.pp_scope = pp_scope

      self.file   = file
      self.parser = parser

      self.container = TestScope(pp_scope.get_inst(), pp_scope.get_expr())

      self.elem_parser = ElemParser(file, parser, self.container)

   #######################################################
   def parse_scope(self, scope):
      return PreprocessScopeParser(self.file, scope, self.parser).parse()
         
   #######################################################
   def parse(self):
      for line in self.pp_scope.get_lines():
         self.elem_parser.parse_line(line)

      if self.elem_parser.incompleted_elem_def():
         fatal(self.file, self.pp_scope.get_lines()[-1], \
            "incompleted " + self.parser.get_type_name() + " definition")

      for scope in self.elem_parser.get_unhandled_sub_scopes():
         self.container.add_sub_scope(self.parse_scope(scope))

      for scope in self.pp_scope.get_elses():
         self.container.add_sibling(self.parse_scope(scope))
         
      return self.container

##########################################################