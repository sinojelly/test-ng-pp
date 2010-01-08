
#ifndef __TESTNGPP_SIMPLE_TEST_CASE_RESULT_REPORTER_H
#define __TESTNGPP_SIMPLE_TEST_CASE_RESULT_REPORTER_H

#include <testngpp/testngpp.h>

#include <testngpp/runner/TestCaseListener.h>
#include <testngpp/runner/TestCaseResultReporter.h>

TESTNGPP_NS_START

struct SimpleTestCaseResultReporterImpl;

struct SimpleTestCaseResultReporter
   : public TestCaseResultReporter
   , public TestCaseListener
{
	SimpleTestCaseResultReporter();
	~SimpleTestCaseResultReporter();

   unsigned int getTestCaseResult(TestCaseInfoReader*) const;

   void addCaseError(TestCaseInfoReader*, const std::string&);
   void addCaseCrash(TestCaseInfoReader*);
   void addCaseFailure(TestCaseInfoReader*, const AssertionFailure&);
   void startTestCase(TestCaseInfoReader*);
   void endTestCase(TestCaseInfoReader*);

private:
   SimpleTestCaseResultReporterImpl * This;
};

TESTNGPP_NS_END

#endif

