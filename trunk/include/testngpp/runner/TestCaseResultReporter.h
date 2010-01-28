
#ifndef __TESTNGPP_TEST_CASE_RESULT_REPORTER_H
#define __TESTNGPP_TEST_CASE_RESULT_REPORTER_H

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestCaseInfoReader;

struct TestCaseResultReporter
{
   enum {
      TR_SUCCESS = 0,
      TR_FAILED  = 1,
      TR_ERROR   = 2,
      TR_CRASHED = 3,
      TR_UNKNOWN = 4
   };
   
   virtual unsigned int getTestCaseResult(TestCaseInfoReader*) const = 0;

   virtual ~TestCaseResultReporter() {}
};

TESTNGPP_NS_END

#endif

