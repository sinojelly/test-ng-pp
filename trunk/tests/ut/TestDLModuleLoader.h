

#include <iostream>

#include <cxxtest/TestSuite.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/ResourceCheckPoint.h>

#include <testngpp/runner/DLModuleLoader.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestDLModuleLoader: public CxxTest::TestSuite
{
private:

   TESTNGPP_RCP checkpoint;

   DLModuleLoader* loader;

public:

   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();

      loader = new DLModuleLoader();
   }
   void tearDown()
   {
      delete loader;

      try {
        TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
      }
      catch(std::exception& ex)
      {
        std::cerr << ex.what() << std::endl;
      }
   }

   void testShouldHaveNoMemoryLeakage()
   {
      StringList searchingPaths;
      //loader->load(searchingPaths, "libmockcpp-ut-TestAny.so");
      loader->load(searchingPaths, "../../samples/libsample.so");
      loader->unload();
   }
};
