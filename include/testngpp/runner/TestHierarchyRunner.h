
#ifndef __TESTNGPP_TEST_HIERARCHY_RUNNER_H
#define __TESTNGPP_TEST_HIERARCHY_RUNNER_H

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestCaseHierarchy;
struct TestFixtureResultCollector;

struct TestHierarchyRunner
{
	virtual void run ( TestCaseHierarchy*
                    , TestFixtureResultCollector* ) = 0;

   virtual ~TestHierarchyRunner() {}
};

TESTNGPP_NS_END

#endif

