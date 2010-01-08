
#ifndef __TESTNGPP_TEST_FIXTURE_SANDBOX_RUNNER_H
#define __TESTNGPP_TEST_FIXTURE_SANDBOX_RUNNER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TestFixtureRunner.h>

TESTNGPP_NS_START

struct TestFixtureDesc;
struct TestCaseRunner;
struct TestFixtureResultCollector;
struct TestFixtureSandboxRunnerImpl;

struct TestFixtureSandboxRunner
   : public TestFixtureRunner
{
   TestFixtureSandboxRunner(
       unsigned int maxCurrentProcess
       , TestCaseRunner*);

   ~TestFixtureSandboxRunner();

	void run ( TestFixtureDesc*
            , TestFixtureResultCollector*
            , const TestCaseFilter*); 

private:
   TestFixtureSandboxRunnerImpl* This;
};

TESTNGPP_NS_END

#endif

