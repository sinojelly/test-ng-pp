
#ifndef __TESTNGPP_AND_COMPOSITE_TESTCASE_FILTER_H
#define __TESTNGPP_AND_COMPOSITE_TESTCASE_FILTER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TestCaseFilter.h>

TESTNGPP_NS_START

struct TestCaseInfoReader;
struct AndCompositeTestCaseFilterImpl;

////////////////////////////////////////////////////////
struct AndCompositeTestCaseFilter 
   : public TestCaseFilter
{
   AndCompositeTestCaseFilter();
   ~AndCompositeTestCaseFilter();

   void addFilter(const TestCaseFilter* filter, bool isComposite);
   bool isCaseMatch(const TestCaseInfoReader* testcase) const;

private:
   AndCompositeTestCaseFilterImpl * This;

};

////////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

