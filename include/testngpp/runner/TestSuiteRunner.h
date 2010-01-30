
#ifndef __TESTNGPP_TEST_SUITE_RUNNER_H
#define __TESTNGPP_TEST_SUITE_RUNNER_H

#include <string>

#include <testngpp/testngpp.h>
#include <testngpp/utils/StringList.h>

TESTNGPP_NS_START

struct TestSuiteLoader;
struct TestFilter;
struct TestFixtureRunner;
struct TestResultCollector;
struct TestSuiteRunnerImpl;

struct TestSuiteRunner
{
	TestSuiteRunner( TestSuiteLoader*
                  , TestFixtureRunner*
                  , TestResultCollector* );
	~TestSuiteRunner();

	void run( const StringList& searchingPaths
           , const std::string& path
           , const TestFilter*);

private:
	TestSuiteRunnerImpl* This;

};

TESTNGPP_NS_END

#endif

