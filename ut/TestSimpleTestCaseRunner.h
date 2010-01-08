
#include <cxxtest/TestSuite.h> 
#include <testcpp/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testcpp/internal/TestCase.h>
#include <testcpp/TestFixture.h>
#include <testcpp/internal/TestFixtureDesc.h>
#include <testcpp/runner/SimpleTestCaseRunner.h>
#include <testcpp/runner/TestCaseResultCollector.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestSimpleTestCaseRunner: public CxxTest::TestSuite
{
   struct MyTestCase : public TestCase
   {
      MyTestCase( const std::string& nameOfCase
           , const std::string& nameOfFixture
           , const std::string& file
           , unsigned int line)
           : TestCase(nameOfCase, nameOfFixture, file, line)
      {}

      void run() {}
   };

private:
   TESTCPP_RCP checkpoint;

   TestFixtureDesc* desc;
   TestCase* testcase[1];

   MockObject<TestFixture> fixture;

   MockObject<TestCaseResultCollector> collector;
public:

   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();
		testcase[0] = new MyTestCase("testCase1", "TestNothing", "TestNothing.h", 11);
      desc = new TestFixtureDesc("TestNothing", "TestNothing.h", fixture, testcase, 1);

      MOCK_METHOD(collector, startTestCase)
         .defaults()
         .with(eq((TestCaseInfoReader*)testcase[0]));

      MOCK_METHOD(collector, endTestCase)
         .defaults()
         .with(eq((TestCaseInfoReader*)testcase[0]));

      MOCK_METHOD(collector, startTestCase)
         .expects(once())
         .before(fixture, "setUp")
         .with(eq((TestCaseInfoReader*)testcase[0]))
         .id("startTestCase");

      MOCK_METHOD(fixture, tearDown)
         .expects(once())
         .after("setUp")
         .id("tearDown");

      MOCK_METHOD(collector, endTestCase)
         .expects(once())
         .with(eq((TestCaseInfoReader*)testcase[0]))
         .after(fixture, "tearDown");
   }

   void tearDown()
   {
		delete testcase[0];
      delete desc;

      fixture.reset();
      collector.reset();

      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToReportStartAndEndEvent()
   {
      MOCK_METHOD(fixture, setUp)
         .expects(once())
         .id("setUp");

      ////////////////////////////////////////////////////////
      SimpleTestCaseRunner runner;

      runner.run(desc, testcase[0], collector);

      fixture.verify();
      collector.verify();
   }

   void testShouldBeAbleToReportAssertionEvent()
   {
      MOCK_METHOD(fixture, setUp)
         .expects(once())
         .will(throws(AssertionFailure("TestNothing.h", 13, "Assertion Failed")))
         .id("setUp");

      MOCK_METHOD(collector, addCaseFailure)
         .expects(once());

      ////////////////////////////////////////////////////////
      SimpleTestCaseRunner runner;

      runner.run(desc, testcase[0], collector);

      fixture.verify();
   }

   void testShouldBeAbleToReportStdError()
   {
      MOCK_METHOD(fixture, setUp)
         .expects(once())
         .will(throws(Exception(13, "TestNothing.h", "Exception")))
         .id("setUp");

      MOCK_METHOD(collector, addCaseError)
         .expects(once())
         .with(any(), contains("Exception"));

      ////////////////////////////////////////////////////////
      SimpleTestCaseRunner runner;

      runner.run(desc, testcase[0], collector);

      fixture.verify();
   }

   void testShouldBeAbleToReportUnknownError()
   {
      MOCK_METHOD(fixture, setUp)
         .expects(once())
         .will(throws(1))
         .id("setUp");

      MOCK_METHOD(collector, addCaseError)
         .expects(once());

      ////////////////////////////////////////////////////////
      SimpleTestCaseRunner runner;

      runner.run(desc, testcase[0], collector);

      fixture.verify();
   }
};
