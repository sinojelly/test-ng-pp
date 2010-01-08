#ifndef __TESTNGPP_TEST_RUNNER_CONTEXT_H
#define __TESTNGPP_TEST_RUNNER_CONTEXT_H

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestResultReporter;
struct TestSuiteResultReporter;
struct TestCaseResultReporter;
struct TestListener;

struct TestRunnerContext
{
   virtual TestResultReporter* getTestResultReporter() const = 0;
   virtual TestSuiteResultReporter* getTestSuiteResultReporter() const = 0;
   virtual TestCaseResultReporter* getTestCaseResultReporter() const = 0;

   virtual void registerTestListener(TestListener* listener) = 0;

   virtual ~TestRunnerContext() {}
};

TESTNGPP_NS_END

#endif

