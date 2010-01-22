#!/usr/bin/python

#####################################
class Scope:
   ##################################
   def __init__(self, parent, name, expr):
      self.parent = parent
      self.name  = name
      self.expr  = expr
      self.lines = []
      self.elses = []

   ##################################
   def get_parent(self):
      return self.parent

   ##################################
   def get_name(self):
      return self.name

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
