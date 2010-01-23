
##########################################
class TestCase:
   ########################################
   def __init__(self, name):
      self.name = name
      self.file = "a.h"
      self.line = 100

   ########################################
   def get_name(self):
      return self.name

   ########################################
   def get_file_name(self):
      return self.file

   ########################################
   def get_line_number(self):
      return self.line

