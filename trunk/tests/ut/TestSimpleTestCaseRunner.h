
#include <cxxtest/TestSuite.h> 
#include <testngppst/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/TestFixture.h>
#include <testngpp/internal/TestCase.h>
#include <testngpp/internal/Warning.h>
#include <testngpp/internal/TestFixtureDesc.h>
#include <testngpp/listener/TestCaseResultCollector.h>

#include <testngpp/runner/SimpleTestCaseRunner.h>

#include <FTestFixture1.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestSimpleTestCaseRunner: public CxxTest::TestSuite
{

private:
   TESTNGPPST_RCP checkpoint;

   FTestFixture1 f;

   TestCaseInfoReader* testcase;

public:

   void setUp()
   {
      checkpoint = TESTNGPPST_SET_RESOURCE_CHECK_POINT();
      f.setUp();

      testcase = f.testcase[0];

      MOCK_METHOD(f.collector, startTestCase)
         .defaults()
         .with(eq((const TestCaseInfoReader*)testcase));

      MOCK_METHOD(f.collector, endTestCase)
         .defaults()
         .with(eq((const TestCaseInfoReader*)testcase));

      MOCK_METHOD(f.collector, startTestCase)
         .expects(once())
         .before(f.fixture, "setUp")
         .with(eq((const TestCaseInfoReader*)testcase))
         .id("startTestCase");

      MOCK_METHOD(f.fixture, tearDown)
         .expects(once())
         .after("setUp")
         .id("tearDown");

      MOCK_METHOD(f.collector, endTestCase)
         .expects(once())
         .with(eq((const TestCaseInfoReader*)testcase))
         .after(f.fixture, "tearDown");
   }

   void tearDown()
   {
      f.tearDown();

      TESTNGPPST_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void run()
   {
      SimpleTestCaseRunner runner(0);

      runner.run(f.testcase[0], f.collector);

      f.verify();
   }

   void testShouldBeAbleToReportStartAndEndEvent()
   {
      MOCK_METHOD(f.fixture, setUp)
         .expects(once())
         .id("setUp");

      run();
   }

   void testShouldBeAbleToReportAssertionEvent()
   {
      MOCK_METHOD(f.fixture, setUp)
         .expects(once())
         .will(throws(AssertionFailure("TestNothing.h", 13, "Assertion Failed")))
         .id("setUp");

      MOCK_METHOD(f.collector, addCaseFailure)
         .expects(once());

      run();
   }

   void testShouldBeAbleToReportStdError()
   {
      MOCK_METHOD(f.fixture, setUp)
         .expects(once())
         .will(throws(TESTNGPP_NS::Warning("TestNothing.h", 13, "Exception")))
         .id("setUp");

      MOCK_METHOD(f.collector, addCaseError)
         .expects(once())
         .with(any(), contains("Exception"));

      run();
   }

   void testShouldBeAbleToReportUnknownError()
   {
      MOCK_METHOD(f.fixture, setUp)
         .expects(once())
         .will(throws(1))
         .id("setUp");

      MOCK_METHOD(f.collector, addCaseError)
         .expects(once());

      run();
   }
};
