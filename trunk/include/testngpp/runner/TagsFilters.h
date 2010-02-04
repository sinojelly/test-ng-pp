
#ifndef __TESTNGPP_TAGS_FILTERS_H
#define __TESTNGPP_TAGS_FILTERS_H

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestCaseFilter;
struct TestCase;
struct TagsFiltersImpl;

////////////////////////////////////////////////////////
struct TagsFilters
{
   TagsFilters();
   ~TagsFilters();

   void addNextFilter(TestCaseFilter*);

   void startOnNext();

   bool shouldRun(const TestCase* testcase) const;
   bool shouldReport(const TestCase* testcase) const;
   bool shouldStore(const TestCase* testcase) const;
   bool hasBeenRunAlready(const TestCase* testcase) const;

private:
   TagsFiltersImpl * This;
};

////////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

