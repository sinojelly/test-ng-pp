
#ifndef __TESTNGPP_NOT_TESTCASE_FILTER_H
#define __TESTNGPP_NOT_TESTCASE_FILTER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TestCaseFilter.h>

TESTNGPP_NS_START

struct TestCase;
struct NotTestCaseFilterImpl;

////////////////////////////////////////////////////////
struct NotTestCaseFilter 
   : public TestCaseFilter
{
   NotTestCaseFilter(TestCaseFilter*, bool);
   ~NotTestCaseFilter();

   bool isCaseMatch(const TestCase* testcase) const;

private:
   NotTestCaseFilterImpl * This;

};

////////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

