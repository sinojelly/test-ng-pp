
#ifndef __TESTNGPP_OR_COMPOSITE_TESTCASE_FILTER_H
#define __TESTNGPP_OR_COMPOSITE_TESTCASE_FILTER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TestCaseFilter.h>

TESTNGPP_NS_START

struct TestCaseInfoReader;
struct OrCompositeTestCaseFilterImpl;

////////////////////////////////////////////////////////
struct OrCompositeTestCaseFilter 
   : public TestCaseFilter
{
   OrCompositeTestCaseFilter();
   ~OrCompositeTestCaseFilter();

   void addFilter(const TestCaseFilter* filter, bool isComposite);
   bool isCaseMatch(const TestCaseInfoReader* testcase) const;

private:
   OrCompositeTestCaseFilterImpl * This;

};

////////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

