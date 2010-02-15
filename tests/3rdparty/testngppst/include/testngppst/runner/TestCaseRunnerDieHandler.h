
#ifndef __TESTNGPPST_TESTCASE_RUNNER_DIE_HANDLER_H
#define __TESTNGPPST_TESTCASE_RUNNER_DIE_HANDLER_H

#include <testngppst/testngppst.h>

TESTNGPPST_NS_START

struct TestCase;
struct TestCaseResultCollector;

struct TestCaseRunnerDieHandler
{
	void die(TestCase*, TestCaseResultCollector*);
};

TESTNGPPST_NS_END

#endif

