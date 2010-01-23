#!/usr/bin/python

#####################################
class PreprocessScope:
   ##################################
   def __init__(self, file, line, parent, inst, expr):
      # Parent Scope
      self.parent = parent 

      # Instrution
      self.inst   = inst   

      # Expression
      self.expr   = expr   

      # Lines in current scope (Tag, Scope, Unknown)
      self.lines  = []     

      # sibling scopes (only #if, #ifndef, #ifdef has this fields)
      # ( contains #elif #else scope )
      self.elses  = []     

      # file name
      self.file   = file
      self.line   = line

   ##################################
   def get_file_name(self):
      return self.file

   ##################################
   def get_line_number(self):
      return self.line

   ##################################
   def get_parent(self):
      return self.parent

   ##################################
   def get_inst(self):
      return self.inst

   ##################################
   def get_lines(self):
      return self.lines

   ##################################
   def get_elses(self):
      return self.elses

   ##################################
   def get_expr(self):
      return self.expr

#####################################
