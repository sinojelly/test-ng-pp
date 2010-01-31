
#ifndef __TESTNGPP_TEST_HIERARCHY_SANDBOX_RUNNER_H
#define __TESTNGPP_TEST_HIERARCHY_SANDBOX_RUNNER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TestHierarchyRunner.h>

TESTNGPP_NS_START

struct TestCaseRunner;
struct TestHierarchySandboxRunnerImpl;

struct TestHierarchySandboxRunner
   : public TestHierarchyRunner
{
   TestHierarchySandboxRunner
         ( unsigned int maxCurrentProcess
         , TestCaseRunner*);

   ~TestHierarchySandboxRunner();

	void run ( TestCaseHierarchy*
            , TestFixtureResultCollector*
            , const TestCaseFilter*); 

private:
   TestHierarchySandboxRunnerImpl* This;
};

TESTNGPP_NS_END

#endif

