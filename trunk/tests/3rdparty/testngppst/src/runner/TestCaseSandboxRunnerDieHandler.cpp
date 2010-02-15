
#include <testngppst/win32/Die.h>
#include <testngppst/runner/TestCaseRunnerDieHandler.h>

TESTNGPPST_NS_START

void TestCaseRunnerDieHandler::
die(TestCase* testcase, TestCaseResultCollector* collector)
{
   throw Die();
}

TESTNGPPST_NS_END
