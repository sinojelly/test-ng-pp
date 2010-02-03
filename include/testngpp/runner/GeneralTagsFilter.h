
#ifndef __TESTNGPP_GENERAL_TAGS_FILTER_H
#define __TESTNGPP_GENERAL_TAGS_FILTER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TestCaseFilter.h>

TESTNGPP_NS_START

struct NameMatcher;
struct TestCase;

struct GeneralTagsFilterImpl;

////////////////////////////////////////////////////////
struct GeneralTagsFilter
   : public TestCaseFilter
{
   GeneralTagsFilter(NameMatcher*);
   ~GeneralTagsFilter();

   bool isCaseMatch(const TestCase* testcase) const;

private:
   GeneralTagsFilterImpl * This;

};

////////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

