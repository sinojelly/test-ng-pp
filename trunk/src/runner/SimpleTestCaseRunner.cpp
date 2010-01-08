
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestCaseResultCollector.h>
#include <testngpp/internal/TestCase.h>
#include <testngpp/internal/TestFixtureDesc.h>
#include <testngpp/TestCaseSuccess.h>

TESTNGPP_NS_START

#define __RUN(block) try block \
   catch(TESTNGPP_NS::TestCaseSuccess& e) \
   { \
   } \
   catch(AssertionFailure& failure) \
   { \
      collector->addCaseFailure(testcase, failure); \
   } \
   catch(std::exception& e) \
   { \
      collector->addCaseError(testcase, e.what()); \
   } \
   catch(...) \
   { \
      collector->addCaseError(testcase, "Unknown Exception"); \
   }

void SimpleTestCaseRunner::run(TestFixtureDesc* desc
      , TestCase* testcase
      , TestCaseResultCollector* collector)
{
   collector->startTestCase(testcase);

   __RUN({
      testcase->setUp();
      testcase->run();
   });

   __RUN({
      testcase->tearDown();
   });

   collector->endTestCase(testcase);
}

TESTNGPP_NS_END

