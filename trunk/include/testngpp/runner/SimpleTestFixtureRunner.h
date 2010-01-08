
#ifndef __TESTNGPP_SIMPLE_TEST_FIXTURE_RUNNER_H
#define __TESTNGPP_SIMPLE_TEST_FIXTURE_RUNNER_H

#include <testngpp/runner/TestFixtureRunner.h>

TESTNGPP_NS_START

///////////////////////////////////////////////////////
struct SimpleTestFixtureRunnerImpl;
struct TestFixtureDesc;
struct TestFixtureResultCollector;
struct TestCaseFilter;
struct TestCaseRunner;

struct SimpleTestFixtureRunner : public TestFixtureRunner
{
	SimpleTestFixtureRunner(TestCaseRunner* runner);
	~SimpleTestFixtureRunner();

	void run(TestFixtureDesc* fixture
      , TestFixtureResultCollector* resultCollector
      , const TestCaseFilter* filter);

private:
	SimpleTestFixtureRunnerImpl * This;
};

///////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

