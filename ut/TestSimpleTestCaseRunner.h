
#include <cxxtest/TestSuite.h> 
#include <testngpp/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/internal/TestCase.h>
#include <testngpp/TestFixture.h>
#include <testngpp/internal/TestFixtureDesc.h>
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestCaseResultCollector.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestSimpleTestCaseRunner: public CxxTest::TestSuite
{
   struct MyTestCase : public TestCase
   {
      MyTestCase(TestFixture* fixture
           , const std::string& nameOfCase
           , const std::string& nameOfFixture
           , const std::string& file
           , unsigned int line)
           : m_fixture(fixture)
           , TestCase(nameOfCase, nameOfFixture, file, line)
      {}

      void run() {}
      void setUp()
      {
         m_fixture->setUp();
      }
      void tearDown()
      {
         m_fixture->tearDown();
      }
     
      TestFixture* m_fixture;
   };

private:
   TESTNGPP_RCP checkpoint;

   TestFixtureDesc* desc;
   TestCase* testcase[1];

   MockObject<TestFixture> fixture;
   MockObject<TestCaseResultCollector> collector;
public:

   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();
		testcase[0] = new MyTestCase(fixture, "testCase1", "TestNothing", "TestNothing.h", 11);
      desc = new TestFixtureDesc("TestNothing", "TestNothing.h", testcase, 1);

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

      TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
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
