#!/usr/bin/python

from Phase1Result import *

from PreprocessScope import PreprocessScope

from TestScope import TestScope

##########################################################
class PreprocessScopeParser:
   #######################################################
   def __init__(self, file, pp_scope, is_elem_def, elem_parser_cls, type_name):

      self.file = file

      self.pp_scope = pp_scope

      self.scopes = []

      self.test_scope = TestScope(pp_scope.get_inst(), pp_scope.get_expr())

      self.parser = None

      self.is_elem_def = is_elem_def
      self.elem_parser_cls = elem_parser_cls

      # "fixture" "testcase"
      self.type_name = type_name

   #######################################################
   def handle_tag(self, tag):
      pass

   #######################################################
   def look_for_elem(self, line):
      if isinstance(line, Tag):
         self.handle_tag(line)
         return

      if isinstance(line, PreprocessScope):
         self.scopes.append(line)
         return

      elem_name = self.is_elem_def (line)
      if elem_name:
         self.parser = \
            self.elem_parser_cls(elem_name, \
               self.pp_scope.get_file_name(), \
               line.get_line_number())

   #######################################################
   def parse_line(self, line):
      if self.parser:
         elem = self.parser.parse(line)
         if elem:
            self.test_scope.add_elem(elem)
            self.parser = None
         return

      self.look_for_elem(line)      
      
   #######################################################
   def parse_scope(self, scope):
      return PreprocessScopeParser(self.file, scope, \
         self.is_elem_def, \
         self.elem_parser_cls, \
         self.type_name).parse()
         
   #######################################################
   def parse(self):
      for line in self.pp_scope.get_lines():
         self.parse_line(line)

      if self.parser:
         fatal(self.file, self.pp_scope.get_lines()[-1], \
            "incompleted " + self.type_name + " definition")

      # sub scope
      for scope in self.scopes:
         self.test_scope.add_sub_scope(self.parse_scope(scope))

      for scope in self.pp_scope.get_elses():
         self.test_scope.add_sibling(self.parse_scope(scope))
         
      return self.test_scope

##########################################################
