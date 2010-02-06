
#include <cxxtest/TestSuite.h> 
#include <testngpp/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/internal/TestCase.h>
#include <testngpp/runner/TestCaseSandbox.h>
#include <testngpp/runner/EnvironmentCleaner.h>
#include <testngpp/runner/TestCaseRunner.h>
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestCaseResultCollector.h>
#include <testngpp/runner/TestCaseSandboxResultDecoder.h>
#include <testngpp/TestFixture.h>
#include <testngpp/internal/TestFixtureDesc.h>

#include <FTestFixture1.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestTestCaseSandbox: public CxxTest::TestSuite
{
private:
   TESTNGPP_RCP checkpoint;

private:

   FTestFixture1 f;

public:
   MockObject<EnvironmentCleaner> cleaner;

   SimpleTestCaseRunner runner;

   TestCaseSandbox* sandbox;

public:

   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();

      f.setUp();

      MOCK_METHOD(f.fixture, setUp).defaults();
      MOCK_METHOD(f.fixture, tearDown).defaults();

      MOCK_METHOD(cleaner, cleanUp).defaults();

      MOCK_METHOD(f.collector, startTestCase).expects(once());
      MOCK_METHOD(f.collector, endTestCase).expects(once());

   }

   void tearDown()
   {
      f.tearDown();

      cleaner.reset();

      TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void verify()
   {
      sandbox->cleanup();
      delete sandbox;

      cleaner.verify();
      f.verify();
   }

   void testShouldBeAbleToReportStartAndEndEvent()
   {
      //////////////////////////////////////////////////////////
      sandbox = TestCaseSandbox::createInstance(cleaner, 
          f.testcase[0], &runner, f.collector, true);
      TS_ASSERT(sandbox != 0);
      ///////////////////////////////////////////////////////////

      sandbox->handle(); // start
      sandbox->handle(); // end

      verify();
   }

   void TtestShouldBeAbleToReportAssertionFailed()
   {
      MOCK_METHOD(f.fixture,setUp)
          .stubs()
          .will(throws(AssertionFailure("abc.h",1,"assertion failure")));

      //////////////////////////////////////////////////////////
      sandbox = TestCaseSandbox::createInstance(cleaner, 
          f.testcase[0], &runner, f.collector, true);

      TS_ASSERT(sandbox != 0);

      ///////////////////////////////////////////////////////////

      MOCK_METHOD(f.collector, addCaseFailure)
           .expects(once());

      ////////////////////////////////////////////////////////////////

      sandbox->handle(); // start
      sandbox->handle(); // assertionFailure
      sandbox->handle(); // end

      verify();
   }

   void testShouldBeAbleToReportError()
   {
      MOCK_METHOD(f.fixture, setUp).stubs().will(throws(Error("error")));

      //////////////////////////////////////////////////////////
      sandbox = TestCaseSandbox::createInstance(cleaner, 
          f.testcase[0], &runner, f.collector, true);
      TS_ASSERT(sandbox != 0);
      //////////////////////////////////////////////////////////

      MOCK_METHOD(f.collector, addCaseError)
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
          f.testcase[0], &runner, f.collector, true);
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
