
import re

from Message import *

blank_re = re.compile( r'^\s*$', re.UNICODE)
tags_re1 = re.compile( r'^\"(?P<tags>[A-Za-z0-9_\s]*)\"$', re.UNICODE)
tags_re2 = re.compile( r'^(?P<tags>[A-Za-z0-9_\s]*)$', re.UNICODE)

def is_blank(str):
   return blank_re.match(str)

##########################################
class TestCase:
   ########################################
   def __init__(self, name, scope, file, line, tag):
      self.traditional_name = name[0]
      self.name             = name[1]
      self.scope            = scope
      self.file             = file
      self.line             = line
      self.annotations      = {"id":None, "depends":None, "tags":[]}
      self.tags_parsed      = None

      self.tag              = tag
      self.depends          = None
      self.generated        = None

      self.parse_tag(self.tag)

   ########################################
   def parse_tags(self):
      tags = self.annotations['tags']
      matched = tags_re1.match(tags)
      if not matched:
         matched = tags_re2.match(tags)

      if matched:
         self.annotations['tags'] = ["\""+tag+"\""  for tag in re.split(r'\s+', matched.group("tags"))]
         return self.annotations['tags']

      fatal(self.file, self.line, "invalid annoation attribute value of \"tags\"")

   ########################################
   def get_tags(self):
      if self.tags_parsed:
         return self.annotations['tags']

      self.tags_parsed = True
      if len(self.annotations['tags']) == 0:
         return self.annotations['tags']

      return self.parse_tags()
      
   ########################################
   def set_scope(self, scope):
      self.scope = scope

   ########################################
   def has_been_generated(self):
      return self.generated

   ########################################
   def mark_as_generated(self):
      self.generated = True

   ########################################
   def matches_id(self, id):
      return id != None and self.annotations["id"] == id

   ########################################
   def report_cyclic_depend_error(self):
      raw_fatal(self.file, self.line, "cyclic depends.")


   ########################################
   def __get_depends(self):
      if self.annotations["depends"] == None:
         return None

      depends = self.scope.find_testcase_with_id(self.annotations["depends"]) 
      if depends == None:
         raw_fatal(self.file, self.line, "no testcase was specified with id=" + self.annotations["depends"])

      return depends

   ########################################
   def get_depends(self):
      if not self.depends:
         self.depends = self.__get_depends()

      return self.depends
     
   ########################################
   def __setitem__(self, key, value):
      self.annotations[key] = value

   ########################################
   def __getitem__(self, key):
      return self.annotations[key]

   ########################################
   def parse_tag(self, tag):
      if tag == None:
         return 

      if tag.get_value() == None:
         return 

      self.parse_attrs(tag, re.split("\s*,\s*", tag.get_value()))

   ########################################
   def parse_attrs(self, tag, attrs):
      for attr in attrs:
         self.parse_attr(tag, attr)

      self.get_tags()

   ########################################
   def report_dup_key(self, tag, key):
      fatal(self.file, tag, "invalid annotation attribute definition, duplicated \""+key+"\"")

   ########################################
   def parse_attr(self, tag, attr):
      kv = re.split("\s*=\s*", attr)
      if len(kv) != 2:
         fatal(self.file, tag, "invalid annotation attribute definition, use key=value pair")

      if is_blank(kv[0]) or is_blank(kv[1]):
         fatal(self.file, tag, "invalid annotation attribute definition, key or value cannot be null")

      self.set_key_value(tag, kv[0], kv[1])

   ########################################
   def set_key_value(self, tag, key, value):
      try:
         if self[key] != None and len(self[key]) != 0:
            self.report_dup_key(tag, key)

         self[key] = value
      except KeyError:
         fatal(self.file, tag, "invalid annotation attribute definition, undefined key \""+key+"\"")
     
   ########################################
   def show(self):
       content = "TEST(" + self.get_name() + ")"
       print content.encode('utf-8')

   ########################################
   def get_name(self):
      if not self.name:
         return self.traditional_name

      return self.name

   ########################################
   def get_traditional_name(self):
      return self.traditional_name

   ########################################
   def get_file_name(self):
      return self.file

   ########################################
   def get_line_number(self):
      return self.line

##########################################
