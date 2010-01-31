
#ifndef __TESTNGPP_TEST_FIXTURE_RUNNER_H__
#define __TESTNGPP_TEST_FIXTURE_RUNNER_H__

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestFixtureDesc;
struct TestFixtureResultCollector;
struct TestHierarchyRunner;
struct TestCaseFilter;
struct TestFixtureRunnerImpl;

struct TestFixtureRunner
{
   TestFixtureRunner(TestHierarchyRunner* runner);
   ~TestFixtureRunner();

	void run ( TestFixtureDesc*
            , TestFixtureResultCollector*
            , const TestCaseFilter*);

private:
   TestFixtureRunnerImpl* This;
};

TESTNGPP_NS_END

#endif

