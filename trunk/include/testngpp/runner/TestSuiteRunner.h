
#ifndef __TESTNGPP_TEST_SUITE_RUNNER_H
#define __TESTNGPP_TEST_SUITE_RUNNER_H

#include <string>

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestSuiteLoader;
struct TestFilter;
struct TestFixtureRunner;
struct TestResultCollector;
struct TestSuiteRunnerImpl;

struct TestSuiteRunner
{
	TestSuiteRunner(TestSuiteLoader*, TestFixtureRunner* );
	~TestSuiteRunner();

	void run( const std::string& path
           , TestResultCollector*
           , const TestFilter*);

private:
	TestSuiteRunnerImpl* This;

};

TESTNGPP_NS_END

#endif

