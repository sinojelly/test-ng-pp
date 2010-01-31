
#ifndef __TESTNGPP_SIMPLE_TEST_HIERARCHY_RUNNER_H
#define __TESTNGPP_SIMPLE_TEST_HIERARCHY_RUNNER_H

#include <testngpp/runner/TestHierarchyRunner.h>

TESTNGPP_NS_START

///////////////////////////////////////////////////////
struct SimpleTestHierarchyRunnerImpl;
struct TestFixtureResultCollector;
struct TestCaseRunner;

struct SimpleTestHierarchyRunner : public TestHierarchyRunner
{
	SimpleTestHierarchyRunner(TestCaseRunner* runner);
	~SimpleTestHierarchyRunner();

	void run( TestCaseHierarchy* 
           , TestFixtureResultCollector*);

private:
   SimpleTestHierarchyRunnerImpl * This;
};

///////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

