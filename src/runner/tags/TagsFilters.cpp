
#include <vector>

#include <testngpp/runner/AndCompositeTestCaseFilter.h>
#include <testngpp/runner/OrCompositeTestCaseFilter.h>
#include <testngpp/runner/NotTestCaseFilter.h>

#include <testngpp/runner/TestCaseFilter.h>
#include <testngpp/runner/TagsFilters.h>

#include <testngpp/internal/TestCase.h>
#include <testngpp/Error.h>


TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct TagsFiltersImpl
{
   TagsFiltersImpl();
   ~TagsFiltersImpl();

   void addNextFilter(TestCaseFilter*);
   void startOnNext();
   bool shouldRun(const TestCase* testcase) const;
   bool shouldReport(const TestCase* testcase) const;

   std::vector<TestCaseFilter*> filters;
   OrCompositeTestCaseFilter allTagsFilter;
   OrCompositeTestCaseFilter doneTagsFilter; 
   NotTestCaseFilter         notDoneFilter;
   int index;
};

////////////////////////////////////////////////////////
TagsFiltersImpl::
TagsFiltersImpl()
   : notDoneFilter(&doneTagsFilter, false)
{
}

////////////////////////////////////////////////////////
TagsFiltersImpl::
~TagsFiltersImpl()
{
   for(int i=0; i<filters.size(); i++)
   {
     delete filters[i];
   }

   filters.clear();
}

////////////////////////////////////////////////////////
void
TagsFiltersImpl::
addNextFilter(TestCaseFilter* filter)
{
   filters.push_back(filter);
   allTagsFilter.addFilter(filter, false);
}

////////////////////////////////////////////////////////
void
TagsFiltersImpl::
startOnNext() 
{
   index++;

   if(index > filters.size())
   {
      throw Error("internal error");
   }

   if(index > 0)
   {
     doneTagsFilter.addFilter(filters[index-1], false);
   }
}

////////////////////////////////////////////////////////
bool
TagsFiltersImpl::
shouldRun(const TestCase* testcase) const
{
   if(index > filters.size())
   {
      throw Error("internal error");
   }
   
   return filters[index]->isCaseMatch(testcase) &&
          notDoneFilter.isCaseMatch(testcase);
}

////////////////////////////////////////////////////////
bool
TagsFiltersImpl::
shouldReport(const TestCase* testcase) const
{
   return allTagsFilter.isCaseMatch(testcase);
}

////////////////////////////////////////////////////////
TagsFilters::
TagsFilters()
   : This(new TagsFiltersImpl())
{
}

////////////////////////////////////////////////////////
TagsFilters::
~TagsFilters()
{
   delete This;
}

////////////////////////////////////////////////////////
void
TagsFilters::
addNextFilter(TestCaseFilter* filter)
{
   This->addNextFilter(filter);
}

////////////////////////////////////////////////////////
void
TagsFilters::
startOnNext() 
{
   This->startOnNext();
}

////////////////////////////////////////////////////////
bool
TagsFilters::
shouldRun(const TestCase* testcase) const
{
   return This->shouldRun(testcase);
}

////////////////////////////////////////////////////////
bool
TagsFilters::
shouldReport(const TestCase* testcase) const
{
   return This->shouldReport(testcase);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
