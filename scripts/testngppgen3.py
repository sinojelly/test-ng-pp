#!/usr/bin/python

import getopt, sys
import os

from Process import process

##########################################################
class LongOptString:
   def __init__(self, optString, example):
      self.optString = optString
      self.example = example

   def toAssignable(self):
      return self.optString + '='

   def toLongOptString(self):
      return '--' + self.optString

   def toLongAssignable(self):
      return '--' + self.optString + '='

   def toUsage(self):
      return self.toLongAssignable() + self.example

##########################################################
def getAssignableOptStrings(longOpts):
   return [i.toAssignable() for i in longOpts]

##########################################################
def getUsageString(longOpts):
   return " ".join([i.toUsage() for i in longOpts])

##########################################################
def usage(longOpts):
   print "Usage:", sys.argv[0], getUsageString(longOpts), "-o target testcase1 testcase2 ..."

##########################################################
def getOpt(longOpts):
   try:
      return getopt.getopt(sys.argv[1:], 'e:o:', getAssignableOptStrings(longOpts))
   except getopt.GetoptError, err:
      print >> sys.stderr, str(err)
      usage(longOpts)
      sys.exit(2)

##########################################################
longOpts = []

##########################################################
def main():
   optlist, fixtures = getOpt(longOpts)

   target = None
   encoding = 'utf-8'

   for o, a in optlist:
      if o == "-o":
         target = a
      if o == "-e":
         encoding = a

   if target == None:
      usage(longOpts)
      sys.exit(1)

   absFixtures = []
   for fixture in fixtures:
      if os.path.isabs(fixture):
         absFixtures.append(fixture)
      else:
         absFixtures.append(os.path.join(os.getcwd(), fixture))

   process(target, absFixtures, encoding)

##########################################################
if __name__ == "__main__":
   main()

