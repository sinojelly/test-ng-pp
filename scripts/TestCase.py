
##########################################
class TestCase:
   ########################################
   def __init__(self, name, file, line):
      self.id   = name[0]
      self.name = name[1]
      self.file = file
      self.line = line

   def show(self):
       content = "TEST(" + self.name + ")"
       print content.encode('utf-8')

   ########################################
   def get_name(self):
      if not self.name:
         return self.id

      return self.name

   ########################################
   def get_id(self):
      return self.id

   ########################################
   def get_file_name(self):
      return self.file

   ########################################
   def get_line_number(self):
      return self.line

##########################################
