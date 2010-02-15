
#if defined(_MSC_VER)
#include <windows.h>
#include <testngppst/win32/die.h>
#endif

#include <testngppst/ExceptionKeywords.h>

#include <testngppst/internal/TestCase.h>
#include <testngppst/internal/TestFixtureDesc.h>

#include <testngppst/listener/TestCaseResultCollector.h>

#include <testngppst/runner/TestCaseRunnerDieHandler.h>
#include <testngppst/runner/SimpleTestCaseRunner.h>
#include <testngppst/runner/SmartTestCaseResultCollector.h>

TESTNGPPST_NS_START

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
   testcase->getFixture()->setCurrentTestCase(testcase, collector);

   smartCollector->startTestCase(testcase);

   __TESTNGPPST_DO

   success = doRun(testcase, smartCollector, handler);

   __TESTNGPPST_CLEANUP

   smartCollector->endTestCase(testcase);
   delete smartCollector;

   __TESTNGPPST_DONE

   return success;
}

TESTNGPPST_NS_END

