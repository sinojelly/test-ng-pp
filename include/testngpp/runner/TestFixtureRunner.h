
#ifndef __TESTNGPP_TEST_FIXTURE_RUNNER_H
#define __TESTNGPP_TEST_FIXTURE_RUNNER_H

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestFixtureDesc;
struct TestFixtureResultCollector;
struct TestCaseFilter;

struct TestFixtureRunner
{
	virtual void run ( TestFixtureDesc*
                    , TestFixtureResultCollector*
                    , const TestCaseFilter* ) = 0;
   virtual ~TestFixtureRunner() {}
};

TESTNGPP_NS_END

#endif

