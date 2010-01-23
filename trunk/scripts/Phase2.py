#!/usr/bin/python

import sys
import re
import os

from Phase1Result import *

from PreprocessScope import *

from Message import *

cpp_re  = re.compile( r'^\s*#\s*(?P<instruction>\w+)(\s+(?P<rest>.*))?$', re.UNICODE)
macro_re = re.compile( r'^\s*[A-Za-z_][A-Za-z0-9_]*\s*$', re.UNICODE)


##########################################################
def create_cont(lines):
   return {'type':'CONT', 'lines':lines}

##########################################################
def create_done(lines, root):
   return {'type':'DONE', 'lines':lines, 'root':root}

##########################################################
def is_scope_inst(inst):
   return inst == "if" or \
          inst == "ifdef" or \
          inst == "ifndef" or \
          inst == "else" or \
          inst == "elif" or \
          inst == "endif"

##########################################################
class BaseScope(PreprocessScope):
   #######################################################
   def __init__(self, file, line, parent, name, expr):
      PreprocessScope.__init__(self, file, line, parent, name, expr)

   #######################################################
   def handle_done(self, result):
      if result['root'] == self:
         return create_cont(result['lines'])

      return result

   #######################################################
   def parse(self, lines):
       lns = lines

       while len(lns) > 0:
          result = self.parse_line(lns)
          if result['type'] == 'DONE':
             return self.handle_done(result)
          else:
             lns = result['lines']

       return create_done([], self)

   #######################################################
   def parse_line(self, lines):
      line = lines[0]

      if isinstance(line, Tag):
         self.lines.append(line)
         return create_cont(lines[1:])

      matched = cpp_re.match(line.get_content())
      if not matched:
         self.lines.append(line) 
         return create_cont(lines[1:])

      return self.parse_cpp_line(lines, matched.group('instruction'), matched.group('rest')) 

   #######################################################
   def parse_cpp_line(self, lines, inst, rest):
      if not is_scope_inst(inst):
         self.lines.append(lines[0]) 
         return create_cont(lines[1:])

      return self.parse_scope_inst(lines, inst, rest)

   #######################################################
   def add_scope(self, scope, lines):
      self.lines.append(scope)
      return scope.parse(lines[1:])


   #######################################################
   def add_ifdef_scope(self, lines, rest, isIfndef):
      matched = macro_re.match(rest)
      if not matched:
         fatal(lines[0], "grammar error: only macro allowed")

      return self.add_scope(IfdefScope(self.file, lines[0], self, rest, isIfndef), lines)

   #######################################################
   def parse_scope_inst(self, lines, inst, rest):
      if inst == "if":
         return self.add_scope(IfScope(self.file, lines[0], self, rest, None), lines)
      elif inst == "ifndef":
         return self.add_ifdef_scope(lines, rest, True)
      elif inst == "ifdef":
         return self.add_ifdef_scope(lines, rest, None)

      return self.parse_other_insts(lines, inst, rest)

   #######################################################
   def parse_other_insts(self, lines, inst, rest):
      fatal(lines[0], "should not be here")

##########################################################
class GlobalScope(BaseScope):
   #######################################################
   def __init__(self, file):
       BaseScope.__init__(self, file, 1, None, "::", None)

   #######################################################
   def parse_other_insts(self, lines, inst, rest):
       fatal(lines[0], "(global) unexpected preprocessor instruction \"" + inst + "\"")

##########################################################
class ConditionScope(BaseScope):
   #######################################################
   def __init__(self, file, line, parent, root, name, expr):
       BaseScope.__init__(self, file, line.get_line_number(), parent, name, expr)
       self.root = root
       if root == None:
          self.root = self

   #######################################################
   def add_elses(self, scope, lines):
      self.root.elses.append(scope)
      return scope.parse(lines[1:])

   #######################################################
   def parse_other_insts(self, lines, inst, rest):
      '''#else, #elif, #endif'''
      if inst == 'endif':
         return create_done(lines[1:], self.root)

      return self.parse_else_scope(lines, inst, rest)

   #######################################################
   def parse_else_scope(self, lines, inst, rest):
      fatal(0, "should not be here")

   #######################################################
   def is_digit(self):
      return self.is_zero() or self.is_one()

   #######################################################
   def is_zero(self):
      return None 

   #######################################################
   def is_one(self):
      return None

##########################################################
zero_re = re.compile( r'^\s*0\s*$' )
one_re  = re.compile( r'^\s*1\s*$' )

##########################################################
def getIfName(isElif):
   if isElif:
      return "elif"
   return "if"

##########################################################
class IfScope(ConditionScope):   
   #######################################################
   def __init__(self, file, line, parent, rest, root):
      self.rest = rest

      isElif = True

      if root == None:
         isElif = None

      self.zero = None
      matched = zero_re.match(rest)
      if matched:
         self.zero = True

      self.one = None
      matched = one_re.match(rest)
      if matched:
         self.one = True

      ConditionScope.__init__(self, file, line, parent, root, getIfName(isElif), rest)

   #######################################################
   def is_zero(self):
      return self.zero

   #######################################################
   def is_one(self):
      return self.one

   def get_scope_cls(self, inst):
      if inst == "elif":
         return IfScope

      return ElseScope

   #######################################################
   def get_scope(self, line, inst, rest):
      return self.get_scope_cls(inst)(self.file, line, self.parent, rest, self.root)

   #######################################################
   def parse_else_scope(self, lines, inst, rest):
      return self.add_elses(self.get_scope(lines[0], inst, rest), lines)

##########################################################
def getIfdefName(isIfndef):
   if isIfndef:
      return "ifndef"

   return "ifdef"

##########################################################
class IfdefScope(ConditionScope):
   #######################################################
   def __init__(self, file, line, parent, rest, isIfndef):
      ConditionScope.__init__(self, file, line, parent, None, getIfdefName(isIfndef), rest)
      self.isIfndef = isIfndef

   #######################################################
   def parse_else_scope(self, lines, inst, rest):
      if inst == "elif":
         fatal(lines[0], "unexpected preprocessor instruction " + inst)

      return self.add_elses(ElseScope(self.file, lines[0], self.parent, rest, self), lines)

##########################################################
class ElseScope(ConditionScope):
   #######################################################
   def __init__(self, file, line, parent, rest, root):
      ConditionScope.__init__(self, file, line, parent, root, "else", None)
   
   #######################################################
   def parse_else_scope(self, lines, inst, rest):
      fatal(lines[0], "unexpected preprocessor instruction " + inst)

   #######################################################
   def is_zero(self):
      return this.root.is_one()

   #######################################################
   def is_one(self):
      return this.root.is_zero()

##########################################################
def phase2(lines, file):
   scope = GlobalScope(file)
   scope.parse(lines)
   return scope


