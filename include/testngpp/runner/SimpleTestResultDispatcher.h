
#ifndef __TESTNGPP_SIMPLE_TEST_RESULT_DISPATCHER_H
#define __TESTNGPP_SIMPLE_TEST_RESULT_DISPATCHER_H

#include <testngpp/testngpp.h>

#include <testngpp/runner/TestResultCollector.h>
#include <testngpp/runner/TestResultDispatcher.h>
#include <testngpp/runner/TestSuiteResultDispatcher.h>
#include <testngpp/runner/TestCaseResultDispatcher.h>
#include <testngpp/runner/TestResultReporter.h>

TESTNGPP_NS_START

struct TestSuiteInfoReader;
struct SimpleTestResultDispatcherImpl;

struct SimpleTestResultDispatcher
   : public TestResultDispatcher
   , public TestSuiteResultDispatcher
   , public TestCaseResultDispatcher
   , public TestResultCollector
{
   SimpleTestResultDispatcher();
   ~SimpleTestResultDispatcher();

   void registerListener(TestListener* listner);

   void registerTestSuiteListener(TestSuiteListener* listener);
   TestSuiteListener* unregisterTestSuiteListener(TestSuiteListener* listener);

   void registerTestCaseListener(TestCaseListener* listener);
   TestCaseListener* unregisterTestCaseListener(TestCaseListener* listener);

   void addCaseError(TestCaseInfoReader*, const std::string&);
   void addCaseCrash(TestCaseInfoReader*);
   void addCaseFailure(TestCaseInfoReader*, const AssertionFailure&);
   void startTestCase(TestCaseInfoReader*);
   void endTestCase(TestCaseInfoReader*);

   void startTestFixture(TestFixtureInfoReader*);
   void endTestFixture(TestFixtureInfoReader*);
   void addFixtureError(TestFixtureInfoReader*, const std::string&);
   void addFixtureFailure(TestFixtureInfoReader*, const AssertionFailure&);

   void startTestSuite(TestSuiteInfoReader*);
   void endTestSuite(TestSuiteInfoReader*);
   void addSuiteError(TestSuiteInfoReader*, const std::string&);

   void startTest();
   void endTest();
   void addError(const std::string&);

private:
   SimpleTestResultDispatcherImpl *This;
};

TESTNGPP_NS_END

#endif

