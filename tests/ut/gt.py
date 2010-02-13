#!/usr/bin/python

import sys
import os.path

template = '''

#include <iostream>

#include <cxxtest/TestSuite.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/ResourceCheckPoint.h>

#include <%s.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class Test%s: public CxxTest::TestSuite
{
private:

   TESTNGPP_RCP checkpoint;

public:

   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();
   }
   void tearDown()
   {
      try {
        TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
      }
      catch(std::exception& ex)
      {
        std::cerr << ex.what() << std::endl;
      }
   }

};
'''

def checkFileExistance(filename):
   if os.path.exists(filename):
      print >> sys.stderr, "file", filename, "exists, please make sure you wanna overwrite it"
      sys.exit(1)

def generate(name, filename):
   FILE = open(filename, "w")
   content = template % (name, name)
   FILE.writelines(content)
   FILE.close()

def generateTestFixture(name):
   filename = "Test" + name + ".h"
   checkFileExistance(filename)
   generate(name, filename)

def usage(prog):
   print "Usage:", prog, "class_name_under_test"

if __name__ == "__main__":
   if len(sys.argv) < 2:
      usage(sys.argv[0])
      sys.exit()

   generateTestFixture(sys.argv[1])
