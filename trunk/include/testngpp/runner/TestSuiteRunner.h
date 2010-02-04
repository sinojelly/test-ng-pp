
#ifndef __TESTNGPP_TEST_SUITE_RUNNER_H
#define __TESTNGPP_TEST_SUITE_RUNNER_H

#include <string>

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestSuiteContext;
struct TestFixtureRunner;
struct TestResultCollector;

struct TestSuiteRunnerImpl;

struct TestSuiteRunner
{
	TestSuiteRunner( TestFixtureRunner*
                  , TestResultCollector* );
	~TestSuiteRunner();

	void run(TestSuiteContext*);

private:
	TestSuiteRunnerImpl* This;

};

TESTNGPP_NS_END

#endif

