
#ifndef __TESTNGPP_TESTCASE_SANDBOX_RESULT_REPORTER_H
#define __TESTNGPP_TESTCASE_SANDBOX_RESULT_REPORTER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TestCaseResultCollector.h>

TESTNGPP_NS_START

struct TestCaseSandboxResultReporterImpl;
struct WrittableChannel;

struct TestCaseSandboxResultReporter
   : public TestCaseResultCollector
{
   TestCaseSandboxResultReporter(WrittableChannel* channel);
   ~TestCaseSandboxResultReporter();

   void addCaseCrash(TestCaseInfoReader*) {}
   void addCaseError(TestCaseInfoReader*, const std::string&); 
   void addCaseFailure(TestCaseInfoReader*, const AssertionFailure&);

   void startTestCase(TestCaseInfoReader*); 
   void endTestCase(TestCaseInfoReader*);

private:
   TestCaseSandboxResultReporterImpl* This;
};

TESTNGPP_NS_END

#endif

