
#include <cxxtest/TestSuite.h> 
#include <testngpp/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/internal/TestCase.h>
#include <testngpp/TestFixture.h>
#include <testngpp/internal/TestFixtureDesc.h>
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestCaseResultCollector.h>

#include <FTestFixture1.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestSimpleTestCaseRunner: public CxxTest::TestSuite
{

private:
   TESTNGPP_RCP checkpoint;

   FTestFixture1 f;

   TestCaseInfoReader* testcase;

public:

   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();
      f.setUp();

      testcase = f.testcase[0];

      MOCK_METHOD(f.collector, startTestCase)
         .defaults()
         .with(eq(testcase));

      MOCK_METHOD(f.collector, endTestCase)
         .defaults()
         .with(eq(testcase));

      MOCK_METHOD(f.collector, startTestCase)
         .expects(once())
         .before(f.fixture, "setUp")
         .with(eq(testcase))
         .id("startTestCase");

      MOCK_METHOD(f.fixture, tearDown)
         .expects(once())
         .after("setUp")
         .id("tearDown");

      MOCK_METHOD(f.collector, endTestCase)
         .expects(once())
         .with(eq(testcase))
         .after(f.fixture, "tearDown");
   }

   void tearDown()
   {
      f.tearDown();

      TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void run()
   {
      SimpleTestCaseRunner runner;

      runner.run(f.desc, f.testcase[0], f.collector);

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
         .will(throws(Exception(13, "TestNothing.h", "Exception")))
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
