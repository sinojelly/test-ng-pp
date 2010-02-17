
#if defined(_MSC_VER)
#include <windows.h>
#include <testngpp/win32/die.h>
#endif

#include <testngpp/ExceptionKeywords.h>

#include <testngpp/utils/StupidTimer.h>

#include <testngpp/internal/TestCase.h>
#include <testngpp/internal/TestFixtureDesc.h>

#include <testngpp/listener/TestCaseResultCollector.h>

#include <testngpp/runner/TestCaseRunnerDieHandler.h>
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/SmartTestCaseResultCollector.h>

TESTNGPP_NS_START

#define __RUN(block) try block \
   catch(AssertionFailure& failure) \
   { \
      hasFailure = true; \
   } \
   catch(std::exception& e) \
   { \
      collector->addCaseError(testcase, e.what()); \
      hasFailure = true; \
   } \
   catch(...) \
   { \
      collector->addCaseError(testcase, "Unknown Exception"); \
      hasFailure = true; \
   }

namespace
{
bool runTest
      ( TestCase* testcase
      , TestCaseResultCollector* collector)
{
   bool hasFailure = false;

   __RUN({
      testcase->setUp();
      testcase->run();
   });

   __RUN({
      testcase->tearDown();
   });

   return !hasFailure;
}

#if defined(_MSC_VER)
int win32TryToRunTest
      ( TestCase* testcase
      , TestCaseResultCollector* collector)
{
	__try
	{
		return runTest(testcase, collector) ? 0:1;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return -1;
	}

	return 0;
}
#endif


bool doRun
   ( TestCase* testcase
   , TestCaseResultCollector* collector
   , TestCaseRunnerDieHandler* handler)
{
#if defined(_MSC_VER)
   int result = tryToRunTest( testcase, collector);
   if(result < 0 && handler != 0)
   {
      handler->die(testcase, collector);
   }

   return result == 0;
#else
   return runTest(testcase, collector);
#endif
}

}

//////////////////////////////////////////
bool SimpleTestCaseRunner::run
      ( TestCase* testcase
      , TestCaseResultCollector* collector
      , bool reportSuccess)
{
   bool success = false;

   TestCaseResultCollector* smartCollector =
         new SmartTestCaseResultCollector(collector, reportSuccess);

   testcase->setFixture();
   testcase->getFixture()->setCurrentTestCase(testcase, smartCollector);

   smartCollector->startTestCase(testcase);

   StupidTimer timer;

   __TESTNGPP_DO

   timer.start();
   success = doRun(testcase, smartCollector, handler);

   __TESTNGPP_CLEANUP

   timeval e = timer.stop();
   smartCollector->endTestCase(testcase, e.tv_sec, e.tv_usec);
   delete smartCollector;

   __TESTNGPP_DONE

   return success;
}

TESTNGPP_NS_END

