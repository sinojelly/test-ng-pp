#ifndef __TESTNGPP_TEST_CASE_FILTER_H
#define __TESTNGPP_TEST_CASE_FILTER_H

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestCaseInfoReader;

////////////////////////////////////////////////////
struct TestCaseFilter
{
   virtual bool isCaseMatch(const TestCaseInfoReader* testcase) const = 0;

   virtual ~TestCaseFilter() {}
};

////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

