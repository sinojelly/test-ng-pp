
#include <testngppst/internal/TestCase.h>
#include <testngppst/listener/TestCaseResultCollector.h>
#include <testngppst/runner/TestCaseRunnerDieHandler.h>

TESTNGPPST_NS_START

void TestCaseRunnerDieHandler::
die(TestCase* testcase, TestCaseResultCollector* collector)
{
   collector->addCaseCrash(testcase);
}

TESTNGPPST_NS_END
