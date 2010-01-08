
#include <cxxtest/TestSuite.h>

#include <mockcpp.hpp>

#include <testcpp/ResourceCheckPoint.h>

#include <GeneralTestFilter.h>
#include <NameMatcher.h>
#include <TestFixtureInfoReader.h>
#include <TestCaseInfoReader.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestGeneralTestFilter: public CxxTest::TestSuite
{
private:

   TESTCPP_RCP checkpoint;

   TestFilter* filter;

   MockObject<NameMatcher> fixtureMatcher;
   MockObject<NameMatcher> caseMatcher;
   MockObject<TestFixtureInfoReader> fixture;
   MockObject<TestCaseInfoReader> testcase;

public:

   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();

      filter = new GeneralTestFilter(fixtureMatcher, caseMatcher);

      std::string fixtureName = "TestFixture";
      std::string caseName = "TestCase";

      fixture.METHOD(TestFixtureInfoReader::getName)
             .defaults().will(returnValue(fixtureName));

      testcase.METHOD(TestCaseInfoReader::getName)
             .defaults().will(returnValue(caseName));

      testcase.METHOD(TestCaseInfoReader::getNameOfFixture)
             .defaults().will(returnValue(fixtureName));

      fixtureMatcher.METHOD(NameMatcher::matches)
             .defaults().with(eq(fixtureName))
             .will(returnValue(true));

      fixtureMatcher.METHOD(NameMatcher::matches)
             .defaults().with(neq(fixtureName))
             .will(returnValue(false));

      caseMatcher.METHOD(NameMatcher::matches)
             .defaults().with(eq(caseName))
             .will(returnValue(true));

      caseMatcher.METHOD(NameMatcher::matches)
             .defaults().with(neq(caseName))
             .will(returnValue(false));
   }

   void tearDown()
   {
      fixture.reset();
      testcase.reset();
      fixtureMatcher.reset();
      caseMatcher.reset();

      delete filter;

      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToTellIfSpecifiedTestFixtureMatches()
   {
      TS_ASSERT(filter->isFixtureMatch(fixture));
      TS_ASSERT(filter->isCaseMatch(testcase));
   }

   
};
