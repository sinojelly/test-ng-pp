
#include <iostream>
#include <cxxtest/TestSuite.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/ResourceCheckPoint.h>

#include <testngpp/runner/TestSuiteContext.h>
#include <testngpp/runner/TestFilter.h>
#include <testngpp/runner/ModuleTestSuiteLoader.h>
#include <testngpp/runner/DLModuleLoader.h>
#include <testngpp/runner/TagsParser.h>
#include <testngpp/runner/TagsFilters.h>
#include <testngpp/runner/TestResultCollector.h>


USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestTestSuiteContext: public CxxTest::TestSuite
{
private:

   TESTNGPP_RCP checkpoint;

   TestSuiteContext* context;
 
   MockObject<TestResultCollector> collector;
   MockObject<TestFilter> filter;

public:

   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();

      MOCK_METHOD(filter, isFixtureMatch)
         .stubs().will(returnValue(true));
      MOCK_METHOD(filter, isCaseMatch)
         .stubs().will(returnValue(true));

   }

   void tearDown()
   {
      filter.reset();
      collector.reset();

      TESTNGPP_VERIFY_RCP_WITH_ERR_MSG(checkpoint);
   }

   void testShouldNotHaveMemoryLeakage()
   {
      TagsFilters* tagsFilter = TagsParser::parse("*");

      context = new TestSuiteContext
         ( new ModuleTestSuiteLoader(new DLModuleLoader())
         , "libmockcpp-ut-TestAny"
         , collector
         , tagsFilter
         , filter);

      delete tagsFilter;

      delete context;
   }
};
