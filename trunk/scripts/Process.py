#!/usr/bin/python

import sys
import re
import os
import codecs

from Phase1 import phase1
from Phase2 import phase2
from Phase3 import phase3
from Phase4 import phase4

##########################################################
def trimCrlf(line):
   if line.endswith('\r\n'):
      return line[:-2]
   elif line.endswith('\n'):
      return line[:-1]
   return line

##########################################################
def getCleanLines(fixtureFile):
   FILE = codecs.open( fixtureFile, "r", "utf-8" )
   lines = FILE.readlines()

   cleanLines= []
   for line in lines:
      cleanLines.append(trimCrlf(line))

   FILE.close()

   return cleanLines

##########################################################
def processTestFixtureFile(file):
    return phase3(phase2(phase1(getCleanLines(file)), file))

##########################################################
def checkFixtureExistence(fixtureFiles):
   for fixtureFile in fixtureFiles:
      if not os.path.exists(fixtureFile):
         print >> sys.stderr, fixtureFile, "does not exist"
         sys.exit(1)


##########################################################
def process(target, fixtureFiles):
   checkFixtureExistence(fixtureFiles)

   fixtures = []
   for fixtureFile in fixtureFiles:
      fixtures.append(processTestFixtureFile(fixtureFile))

   phase4(target, fixtures)

