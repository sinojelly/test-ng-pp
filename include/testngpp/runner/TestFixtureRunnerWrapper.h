
#ifndef __TESTNGPP_TEST_FIXTURE_RUNNER_WRAPPER_H
#define __TESTNGPP_TEST_FIXTURE_RUNNER_WRAPPER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TestFixtureRunner.h>

TESTNGPP_NS_START

struct TestFixtureDesc;
struct TestFixtureResultCollector;

struct TestFixtureRunnerWrapper
	: public TestFixtureRunner
{
   TestFixtureRunnerWrapper(TestFixtureRunner* runner);
   ~TestFixtureRunnerWrapper();

	void run ( TestFixtureDesc*
            , TestFixtureResultCollector*
            , const TestCaseFilter*);

private:
   TestFixtureRunner* fixtureRunner;
};

TESTNGPP_NS_END

#endif

