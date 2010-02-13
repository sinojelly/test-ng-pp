

#include <iostream>

#include <cxxtest/TestSuite.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/ResourceCheckPoint.h>

#include <testngpp/runner/ModuleTestSuiteLoader.h>
#include <testngpp/runner/DLModuleLoader.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestModuleTestSuiteLoader: public CxxTest::TestSuite
{
private:

   TESTNGPP_RCP checkpoint;

   ModuleTestSuiteLoader *loader;
   TestSuiteDesc* desc;

public:

   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();
      loader = new ModuleTestSuiteLoader(new DLModuleLoader());
      desc = 0;
   }
   void tearDown()
   {
      delete loader;

      TESTNGPP_VERIFY_RCP_WITH_ERR_MSG(checkpoint);
   }

   void testShouldNotHaveMemoryLeakage()
   {
      StringList searchingPaths;
      desc = loader->load(searchingPaths, "libmockcpp-ut-TestAny", 0);
      loader->unload();
   }
};
