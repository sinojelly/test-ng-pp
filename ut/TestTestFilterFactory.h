
#include <cxxtest/TestSuite.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/ResourceCheckPoint.h>

#include <testngpp/runner/TestFilterFactory.h>
#include <testngpp/runner/TestFilter.h>
#include <testngpp/internal/TestFixtureInfoReader.h>
#include <testngpp/internal/TestCaseInfoReader.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestTestFilterFactory: public CxxTest::TestSuite
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
      TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldReturnAWildcardFilterIfNoFilterOptionsSpecified()
   {
      std::list<std::string> filterOptions;

      const TestFilter* filter = TestFilterFactory::getFilter(filterOptions);

      TS_ASSERT(filter != 0);

      /////////////////////////////////////////////////
      MockObject<TestFixtureInfoReader> fixture;

      fixture.METHOD(TestFixtureInfoReader::getName)
             .stubs().will(returnValue(std::string("stupidNameOrWhatever")));

      TS_ASSERT(filter->isFixtureMatch(fixture));

      /////////////////////////////////////////////////
      MockObject<TestCaseInfoReader> testcase;

      testcase.METHOD(TestCaseInfoReader::getName)
             .stubs().will(returnValue(std::string("stupidNameOrWhatever")));

      TS_ASSERT(filter->isCaseMatch(testcase));

      //////////////////////////////////////////////////
      TestFilterFactory::returnFilter(filter);
   }

   void testShouldReturnACapableFilterIfThereAreFilterOptionsSpecified()
   {
      std::list<std::string> filterOptions;
      filterOptions.push_back("Fixture1:testcase0");

      const TestFilter* filter = TestFilterFactory::getFilter(filterOptions);

      TS_ASSERT(filter != 0);

      /////////////////////////////////////////////////
      MockObject<TestFixtureInfoReader> fixture0;

      fixture0.METHOD(TestFixtureInfoReader::getName)
              .stubs().will(returnValue(std::string("Fixture0")));

      TS_ASSERT(!filter->isFixtureMatch(fixture0));

      //////////////////////////////////////////////////
      MockObject<TestFixtureInfoReader> fixture2;

      fixture2.METHOD(TestFixtureInfoReader::getName)
              .stubs().will(returnValue(std::string("Fixture2")));

      TS_ASSERT(!filter->isFixtureMatch(fixture2));

      //////////////////////////////////////////////////
      MockObject<TestFixtureInfoReader> fixture1;

      fixture1.METHOD(TestFixtureInfoReader::getName)
              .stubs().will(returnValue(std::string("Fixture1")));

      TS_ASSERT(filter->isFixtureMatch(fixture1));

      //////////////////////////////////////////////////
      MockObject<TestCaseInfoReader> testcase0;

      testcase0.METHOD(TestCaseInfoReader::getNameOfFixture)
              .stubs().will(returnValue(std::string("Fixture0")));

      testcase0.METHOD(TestCaseInfoReader::getName)
              .stubs().will(returnValue(std::string("testcase0")));

      TS_ASSERT(!filter->isCaseMatch(testcase0));

      //////////////////////////////////////////////////
      MockObject<TestCaseInfoReader> testcase1;

      testcase1.METHOD(TestCaseInfoReader::getNameOfFixture)
              .stubs().will(returnValue(std::string("Fixture1")));

      testcase1.METHOD(TestCaseInfoReader::getName)
              .stubs().will(returnValue(std::string("testcase0")));

      TS_ASSERT(filter->isCaseMatch(testcase1));

      //////////////////////////////////////////////////
      TestFilterFactory::returnFilter(filter);
   }
};
