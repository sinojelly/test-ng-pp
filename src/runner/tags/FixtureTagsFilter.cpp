
#include <list>

#include <testngpp/runner/TagsFilters.h>
#include <testngpp/runner/FixtureTagsFilter.h>

#include <testngpp/internal/TestCase.h>


TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct FixtureTagsFilterImpl
{
   FixtureTagsFilterImpl(TagsFilters* filter);
   ~FixtureTagsFilterImpl();

   void startUp();

   bool shouldRun(const TestCase* testcase) const;
   bool shouldReport(const TestCase* testcase) const;
   bool shouldStore(const TestCase* testcase) const;

   void store(const TestCase* testcase);

   bool hasBeenRunAlready(const TestCase* testcase) const;

   std::list<const TestCase*> preRunTestCases;
   TagsFilters* tagsFilters;
};

namespace
{
   struct CleanupRedundantPreRunTestCases
   {
      CleanupRedundantPreRunTestCases(TagsFilters* filters)
         : tagsFilters(filters)
      {}
      
      bool operator()(const TestCase* testcase)
      {
         return tagsFilters->hasBeenRunAlready(testcase);
      }

      TagsFilters* tagsFilters;
   };
}

////////////////////////////////////////////////////////
FixtureTagsFilterImpl::
FixtureTagsFilterImpl(TagsFilters* filters)
   : tagsFilters(filters)
{
}

////////////////////////////////////////////////////////
FixtureTagsFilterImpl::
~FixtureTagsFilterImpl()
{
}

////////////////////////////////////////////////////////
bool
FixtureTagsFilterImpl::
hasBeenRunAlready(const TestCase* testcase) const
{
   std::list<const TestCase*>::const_iterator i = \
     preRunTestCases.begin();
   for(; i != preRunTestCases.end(); i++)
   {
      if((*i) == testcase)
         return true;
   }

   return false;
}

////////////////////////////////////////////////////////
bool
FixtureTagsFilterImpl::
shouldRun(const TestCase* testcase) const
{
   return tagsFilters->shouldRun(testcase) && 
          !hasBeenRunAlready(testcase);
}

////////////////////////////////////////////////////////
bool
FixtureTagsFilterImpl::
shouldReport(const TestCase* testcase) const
{
   return tagsFilters->shouldReport(testcase);
}

////////////////////////////////////////////////////////
bool
FixtureTagsFilterImpl::
shouldStore(const TestCase* testcase) const
{
   return tagsFilters->shouldStore(testcase) &&
          !hasBeenRunAlready(testcase);
}

////////////////////////////////////////////////////////
void
FixtureTagsFilterImpl::
store(const TestCase* testcase)
{
   if(shouldStore(testcase))
   {
      preRunTestCases.push_back(testcase);
   }
}

////////////////////////////////////////////////////////
void
FixtureTagsFilterImpl::
startUp()
{
   while(1)
   {
      std::list<const TestCase*>::iterator i = \
         std::find_if( preRunTestCases.begin()
                     , preRunTestCases.end()
                     , CleanupRedundantPreRunTestCases(tagsFilters)); 

      if(i == preRunTestCases.end())
      {
         break;
      }

      preRunTestCases.erase(i);
   }
}

////////////////////////////////////////////////////////
FixtureTagsFilter::
FixtureTagsFilter(TagsFilters* filters)
   : This(new FixtureTagsFilterImpl(filters))
{
}

////////////////////////////////////////////////////////
FixtureTagsFilter::
~FixtureTagsFilter()
{
   delete This;
}

////////////////////////////////////////////////////////
bool
FixtureTagsFilter::
shouldRun(const TestCase* testcase) const
{
   return This->shouldRun(testcase);
}

////////////////////////////////////////////////////////
bool
FixtureTagsFilter::
shouldReport(const TestCase* testcase) const
{
   return This->shouldReport(testcase);
}

////////////////////////////////////////////////////////
void
FixtureTagsFilter::
startUp()
{
   return This->startUp();
}

////////////////////////////////////////////////////////
void
FixtureTagsFilter::
testDone(const TestCase* testcase) 
{
   This->store(testcase);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
