
#ifndef __TESTNGPP_SIMPLE_TEST_RESULT_REPORTER_H
#define __TESTNGPP_SIMPLE_TEST_RESULT_REPORTER_H

#include <testngpp/testngpp.h>

#include <testngpp/runner/TestListener.h>
#include <testngpp/runner/TestResultReporter.h>

TESTNGPP_NS_START

struct TestSuiteResultReporter;
struct SimpleTestResultReporterImpl;

struct SimpleTestResultReporter
   : public TestResultReporter
   , public TestListener
{
	SimpleTestResultReporter(TestSuiteResultReporter*);
	~SimpleTestResultReporter();

   unsigned int getNumberOfTestCases() const;
   unsigned int getNumberOfSuccessfulTestCases() const;
   unsigned int getNumberOfUnsuccessfulTestCases() const;
   unsigned int getNumberOfFailedTestCases() const;
   unsigned int getNumberOfErrorTestCases() const;
   unsigned int getNumberOfCrashedTestCases() const;
   unsigned int getNumberOfLoadedSuites() const;
   unsigned int getNumberOfUnloadableSuites() const;

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

   SimpleTestResultReporterImpl* This;
};

TESTNGPP_NS_END

#endif
