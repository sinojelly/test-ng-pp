
#include <cxxtest/TestSuite.h> 
#include <testcpp/ResourceCheckPoint.h>

#include <mockcpp.hpp>

#include <TestFixtureSandboxRunner.h>
#include <TestFixtureDef.h>
#include <SimpleTestCaseRunner.h>
#include <TestFixtureResultCollector.h>
#include <TestCaseFilter.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestTestFixtureSandboxRunner: public CxxTest::TestSuite
{
private:
   TESTCPP_RCP checkpoint;

   MockObject<TestCaseFilter> filter;
   MockObject<TestFixtureResultCollector> collector;
public:
   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();

      filter.METHOD(TestCaseFilter::isCaseMatch)
         .defaults().will(returnValue(true));

   }

   void tearDown()
   {
      collector.reset();
      filter.reset();

      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToRunATestFixture()
   {
      MOCK_METHOD(collector, startTestCase)
               .expects(once())
               .with(eq((TestCaseInfoReader*)&myTestCase1))
               .id("start1");

      MOCK_METHOD(collector, addCaseFailure)
               .expects(once())
               .with(eq((TestCaseInfoReader*)&myTestCase1))
               .after("start1")
               .id("failure1");

      MOCK_METHOD(collector, endTestCase)
               .expects(once())
               .with(eq((TestCaseInfoReader*)&myTestCase1))
               .after("failure1")
               .id("end1");

      MOCK_METHOD(collector, startTestCase)
               .expects(once())
               .with(eq((TestCaseInfoReader*)&myTestCase2))
               .id("start2");

      MOCK_METHOD(collector, addCaseError)
               .expects(once())
               .with(eq((TestCaseInfoReader*)&myTestCase2))
               .after("start2")
               .id("error2");

      MOCK_METHOD(collector, endTestCase)
               .expects(once())
               .with(eq((TestCaseInfoReader*)&myTestCase2))
               .after("error2")
               .id("end2");

      MOCK_METHOD(collector, startTestCase)
               .expects(once())
               .with(eq((TestCaseInfoReader*)&myTestCase3))
               .id("start3");

      MOCK_METHOD(collector, endTestCase)
               .expects(once())
               .with(eq((TestCaseInfoReader*)&myTestCase3))
               .after("start3");

      SimpleTestCaseRunner caseRunner;
      TestFixtureSandboxRunner runner(2, &caseRunner);
      runner.run(&myTestFixtureDesc, collector, filter);
   }
};
