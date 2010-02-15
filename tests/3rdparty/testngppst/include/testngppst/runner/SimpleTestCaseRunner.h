
#ifndef __TESTNGPPST_SIMPLE_TEST_CASE_RUNNER_H
#define __TESTNGPPST_SIMPLE_TEST_CASE_RUNNER_H

#include <testngppst/testngppst.h>
#include <testngppst/runner/TestCaseRunner.h>
#include <testngppst/runner/TestCaseRunnerDieHandler.h>

TESTNGPPST_NS_START

struct SimpleTestCaseRunner
	: public TestCaseRunner
{
	SimpleTestCaseRunner(TestCaseRunnerDieHandler* dieHandler)
		: handler(dieHandler)
	{}

	~SimpleTestCaseRunner()
	{
		if(handler != 0)
		{
			delete handler;
		}
	}

	bool run(TestCase*, TestCaseResultCollector*, bool reportSuccess=true);

private:
    TestCaseRunnerDieHandler* handler;
};

TESTNGPPST_NS_END

#endif

