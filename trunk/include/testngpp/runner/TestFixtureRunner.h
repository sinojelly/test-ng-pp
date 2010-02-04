
#ifndef __TESTNGPP_TEST_FIXTURE_RUNNER_H__
#define __TESTNGPP_TEST_FIXTURE_RUNNER_H__

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestFixtureContext;
struct TestFixtureResultCollector;
struct TestHierarchyRunner;
struct TestCaseFilter;
struct TestFixtureRunnerImpl;

struct TestFixtureRunner
{
   TestFixtureRunner(TestHierarchyRunner* runner);
   ~TestFixtureRunner();

	void run ( TestFixtureContext *
            , TestFixtureResultCollector*
            , const TestCaseFilter*);

private:
   TestFixtureRunnerImpl* This;
};

TESTNGPP_NS_END

#endif

