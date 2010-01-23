
##########################################
class TestCase:
   ########################################
   def __init__(self, name, file, line):
      self.name = name
      self.file = file
      self.line = line

   def show(self):
       content = "TEST(" + self.name + ")"
       print content.encode('utf-8')

   ########################################
   def get_name(self):
      return self.name

   ########################################
   def get_file_name(self):
      return self.file

   ########################################
   def get_line_number(self):
      return self.line

##########################################
