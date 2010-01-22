#!/usr/bin/python

import sys
import re
import os
import codecs

from Phase1Result import *

##########################################################
blank_slc_re  = re.compile( r'^\s*//.*$', re.UNICODE)
tag_re        = re.compile( r'^\s*//\s*@\s*(?P<tag>[A-Za-z]+)\s*(:\s*(?P<value>.*?))?\s*$', re.UNICODE )

slc_re   = re.compile( r'^(?P<content>.*?)//.*$', re.UNICODE )
smlc_re  = re.compile( r'^(?P<content>.*?)/\*.*$', re.UNICODE )
emlc_re  = re.compile( r'^.*?\*/(?P<rest>.*)$', re.UNICODE )

blank_re = re.compile( r'^\s*$', re.UNICODE)

mlc_re   = re.compile( r'^(?P<content>.*)/\*.*\*/(?P<rest>.*)$', re.UNICODE)

##########################################################
def split_by_mlc(line):
   contents = []
   rest = line
   while True:
      matched = mlc_re.match(rest)
      if not matched:
         contents.append(rest)
         return contents
      contents.append(matched.group('content'))
      rest = matched.group('rest')
      if not rest:
         return contents

##########################################################
def is_blank(line):
   return line == None or blank_re.match(line)

##########################################################
def is_blank_slc(line):
   return blank_slc_re.match(line)

##########################################################
class Phase1Parser:
   def __init__(self, lines):
      self.result = []
      self.line_number = 1
      self.lines = lines
      self.in_comment = None

   #######################################################
   def is_tag_of(self, line):
      m = tag_re.match(line)
      if not m:
         return None

      self.result.append(Tag(self.line_number, m.group('tag'), m.group('value')))

      return True

   #######################################################
   def remove_single_line_comment(self, line):
      matched = slc_re.match(line)
      if not matched:
         return line

      return matched.group("content")

   #######################################################
   def try_to_erase_comment(self, line):
      content = " ".join(split_by_mlc(line))
      content_without_slc = self.remove_single_line_comment(content)
      matched = smlc_re.match(content_without_slc)
      if matched:
         self.in_comment = True
         return matched.group("content")

      return content_without_slc

   #######################################################
   def parseLineInComment(self, line):
      if not self.in_comment:
         return None

      matched = emlc_re.match(line)
      if matched:
         self.in_comment = None
         self.parseLineOutOfComment(matched.group('rest'))

      return True

   #######################################################
   def parseLineOutOfComment(self, line):
      if is_blank(line) or self.is_tag_of(line) or is_blank_slc(line):
         return

      content = self.try_to_erase_comment(line)
      if is_blank(content):
         return

      self.result.append(Unknown(self.line_number, content))

   #######################################################
   def parseLine(self, line):
      self.parseLineInComment(line) or self.parseLineOutOfComment(line)

   #######################################################
   def parse(self):
      for line in self.lines:
         self.parseLine(line)
         self.line_number += 1

      return self.result

##########################################################
def phase1(lines):
   return Phase1Parser(lines).parse()


