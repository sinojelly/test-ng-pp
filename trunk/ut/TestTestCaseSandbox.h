
#include <cxxtest/TestSuite.h> 
#include <testcpp/ResourceCheckPoint.h>

#include <mockcpp.hpp>

#include <TestCase.h>
#include <TestCaseSandbox.h>
#include <EnvironmentCleaner.h>
#include <TestCaseRunner.h>
#include <SimpleTestCaseRunner.h>
#include <TestCaseResultCollector.h>
#include <TestCaseSandboxResultDecoder.h>
#include <TestFixture.h>
#include <TestFixtureDesc.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestTestCaseSandbox: public CxxTest::TestSuite
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

public:
   TestFixtureDesc* desc;
   TestCase* testcase[1];

   MockObject<EnvironmentCleaner> cleaner;
   MockObject<TestFixture> fixture;
   MockObject<TestCaseResultCollector> collector;

   SimpleTestCaseRunner runner;

   TestCaseSandbox* sandbox;

public:

   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();
      testcase[0] = new MyTestCase("testCase1", "TestNothing", "TestNothing.h", 11);
      desc = new TestFixtureDesc("TestNothing", "TestNothing.h", fixture, testcase, 1);

      fixture.METHOD(TestFixture::setUp).defaults();
      fixture.METHOD(TestFixture::tearDown).defaults();
      cleaner.METHOD(EnvironmentCleaner::cleanUp).defaults();

      collector.METHOD(TestCaseResultCollector::startTestCase)
               .expects(once());
      collector.METHOD(TestCaseResultCollector::endTestCase)
               .expects(once());

   }

   void tearDown()
   {
      delete testcase[0];
      delete desc;

      collector.reset();
      cleaner.reset();
      fixture.reset();

      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void verify()
   {
      sandbox->cleanup();
      delete sandbox;

      fixture.verify();
      collector.verify();
      cleaner.verify();
   }

   void testShouldBeAbleToReportStartAndEndEvent()
   {
      //////////////////////////////////////////////////////////
      sandbox = TestCaseSandbox::createInstance(cleaner, 
          testcase[0], desc, &runner, collector);
      TS_ASSERT(sandbox != 0);
      ///////////////////////////////////////////////////////////

      sandbox->handle(); // start
      sandbox->handle(); // end

      verify();
   }

   void TtestShouldBeAbleToReportAssertionFailed()
   {
      fixture.METHOD(TestFixture::setUp).stubs().will(throws(AssertionFailure("abc.h",1,"assertion failure")));

      //////////////////////////////////////////////////////////
      sandbox = TestCaseSandbox::createInstance(cleaner, 
          testcase[0], desc, &runner, collector);
      TS_ASSERT(sandbox != 0);
      ///////////////////////////////////////////////////////////

      collector.METHOD(TestCaseResultCollector::addCaseFailure)
               .expects(once());

      ////////////////////////////////////////////////////////////////

      sandbox->handle(); // start
      sandbox->handle(); // assertionFailure
      sandbox->handle(); // end

      verify();
   }

   void testShouldBeAbleToReportError()
   {
      fixture.METHOD(TestFixture::setUp).stubs().will(throws(Error("error")));

      //////////////////////////////////////////////////////////
      sandbox = TestCaseSandbox::createInstance(cleaner, 
          testcase[0], desc, &runner, collector);
      TS_ASSERT(sandbox != 0);
      //////////////////////////////////////////////////////////

      collector.METHOD(TestCaseResultCollector::addCaseError)
               .expects(once());

      sandbox->handle(); // start
      sandbox->handle(); // error
      sandbox->handle(); // end

      verify();
   }

   void testShouldBecomeDeadAfterAllEventsHaveBeenHandled()
   {
      //////////////////////////////////////////////////////////
      sandbox = TestCaseSandbox::createInstance(cleaner, 
          testcase[0], desc, &runner, collector);
      TS_ASSERT(sandbox != 0);
      ///////////////////////////////////////////////////////////

      TS_ASSERT(!sandbox->isDead());

      sandbox->handle(); // start
      TS_ASSERT(!sandbox->isDead());

      sandbox->handle(); // end
      TS_ASSERT(!sandbox->isDead());

      sandbox->handle(); // EOF
      TS_ASSERT(sandbox->isDead());

      verify();
   }
};
