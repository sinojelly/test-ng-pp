
#ifndef __TESTNGPP_TEST_CASE_RUNNER_H
#define __TESTNGPP_TEST_CASE_RUNNER_H

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestFixtureDesc;
struct TestCase;
struct TestCaseResultCollector;

struct TestCaseRunner
{
	virtual void run(TestCase*, TestCaseResultCollector*) = 0;

   virtual ~TestCaseRunner() {}
};

TESTNGPP_NS_END

#endif

