#ifndef __TESTNGPP_TEST_RESULT_MANAGER_H__
#define __TESTNGPP_TEST_RESULT_MANAGER_H__

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestResultReporter;
struct TestSuiteResultReporter;
struct TestCaseResultReporter;
struct TestListener;

struct TestResultManager
{
   virtual TestResultReporter* getTestResultReporter() const = 0;
   virtual TestSuiteResultReporter* getTestSuiteResultReporter() const = 0;
   virtual TestCaseResultReporter* getTestCaseResultReporter() const = 0;

   virtual void registerTestListener(TestListener* listener) = 0;

   virtual ~TestResultManager() {}
};

TESTNGPP_NS_END

#endif

