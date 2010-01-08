#ifndef __TESTNGPP_TEST_RUNNER_H
#define __TESTNGPP_TEST_RUNNER_H

#include <list>
#include <string>

#include <testngpp/testngpp.h>

#include <testngpp/runner/TestRunnerContext.h>

TESTNGPP_NS_START

struct TestRunnerImpl;

struct TestRunner
   : public TestRunnerContext
{
   typedef std::list<std::string> StringList;

public:
   TestRunner();
   ~TestRunner();

   TestResultReporter* getTestResultReporter() const;
   TestSuiteResultReporter* getTestSuiteResultReporter() const;
   TestCaseResultReporter* getTestCaseResultReporter() const;

   void registerTestListener(TestListener* listener);

   int runTests( bool useSandboxRunner
               , unsigned int maxConcurrent
               , const StringList& suitePaths
               , const StringList& listenerNames
               , const StringList& searchingPathsOfListeners
               , const StringList& fixtures);

private:
   TestRunnerImpl * This;

};

TESTNGPP_NS_END

#endif

