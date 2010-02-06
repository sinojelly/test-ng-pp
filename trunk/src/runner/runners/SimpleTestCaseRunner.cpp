
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestCaseResultCollector.h>
#include <testngpp/internal/TestCase.h>
#include <testngpp/internal/TestFixtureDesc.h>

TESTNGPP_NS_START

#define __RUN(block) try block \
   catch(AssertionFailure& failure) \
   { \
      if(collector != 0) collector->addCaseFailure(testcase, failure); \
      hasFailure = true; \
   } \
   catch(std::exception& e) \
   { \
      if(collector != 0)  collector->addCaseError(testcase, e.what()); \
      hasFailure = true; \
   } \
   catch(...) \
   { \
      if(collector != 0) collector->addCaseError(testcase, "Unknown Exception"); \
      hasFailure = true; \
   }

bool SimpleTestCaseRunner::run
      ( TestCase* testcase
      , TestCaseResultCollector* collector)
{
   bool hasFailure = false;

   if(collector != 0) collector->startTestCase(testcase);

   __RUN({
      testcase->setUp();
      testcase->run(true);
   });

   __RUN({
      testcase->tearDown();
   });

   if(collector != 0) collector->endTestCase(testcase);

   return !hasFailure;
}

TESTNGPP_NS_END

