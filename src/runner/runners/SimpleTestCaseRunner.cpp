
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestCaseResultCollector.h>
#include <testngpp/internal/TestCase.h>
#include <testngpp/internal/TestFixtureDesc.h>

TESTNGPP_NS_START

#define __RUN(block) try block \
   catch(AssertionFailure& failure) \
   { \
      collector->addCaseFailure(testcase, failure); \
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

bool SimpleTestCaseRunner::run
      ( TestCase* testcase
      , TestCaseResultCollector* collector
      , bool reportSuccess)
{
   bool hasFailure = false;

   if(hasFailure || reportSuccess)
   {
      collector->startTestCase(testcase);
   }

   __RUN({
      testcase->setUp();
      testcase->run(true);
   });

   __RUN({
      testcase->tearDown();
   });

   if(hasFailure || reportSuccess)
   {
      collector->endTestCase(testcase);
   }

   return !hasFailure;
}

TESTNGPP_NS_END

