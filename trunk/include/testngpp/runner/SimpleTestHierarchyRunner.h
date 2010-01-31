
#ifndef __TESTNGPP_SIMPLE_TEST_HIERARCHY_RUNNER_H
#define __TESTNGPP_SIMPLE_TEST_HIERARCHY_RUNNER_H

#include <testngpp/runner/TestHierarchyRunner.h>

TESTNGPP_NS_START

///////////////////////////////////////////////////////
struct SimpleTestHierarchyRunnerImpl;
struct TestFixtureResultCollector;
struct TestCaseFilter;
struct TestCaseRunner;

struct SimpleTestHierarchyRunner : public TestHierarchyRunner
{
	SimpleTestHierarchyRunner(TestCaseRunner* runner);
	~SimpleTestHierarchyRunner();

	void run( TestCaseHierarchy* 
           , TestFixtureResultCollector* 
           , const TestCaseFilter* );

private:
   SimpleTestHierarchyRunnerImpl * This;
};

///////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

