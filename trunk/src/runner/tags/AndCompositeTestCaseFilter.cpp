
#include <list>
#include <algorithm>

#include <testngpp/runner/AndCompositeTestCaseFilter.h>
#include <testngpp/internal/TestCaseInfoReader.h>

TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct AndCompositeTestCaseFilterImpl
{
   typedef std::pair<const TestCaseFilter*, bool> ValueType;
   typedef std::list<ValueType> Filters;

   Filters filters;

   ~AndCompositeTestCaseFilterImpl();

   void addFilter(const TestCaseFilter* filter, bool isComposite);
   bool isCaseMatch(const TestCaseInfoReader* testcase) const;
};

////////////////////////////////////////////////////////
AndCompositeTestCaseFilterImpl::
~AndCompositeTestCaseFilterImpl()
{
   Filters::iterator i = filters.begin();
   for(; i != filters.end(); i++)
   {
      if((*i).second)
         delete (*i).first;
   }
}

////////////////////////////////////////////////////////
AndCompositeTestCaseFilter::
AndCompositeTestCaseFilter()
   : This(new AndCompositeTestCaseFilterImpl())
{
}

////////////////////////////////////////////////////////
AndCompositeTestCaseFilter::
~AndCompositeTestCaseFilter()
{
   delete This;
}

////////////////////////////////////////////////////
bool AndCompositeTestCaseFilterImpl::
isCaseMatch(const TestCaseInfoReader* testcase) const
{
   Filters::const_iterator i = filters.begin();
   for(; i != filters.end(); i++)
   {
      if(!(*i).first->isCaseMatch(testcase))
         return false;
   }

   return true;
}

////////////////////////////////////////////////////
void AndCompositeTestCaseFilterImpl::
addFilter(const TestCaseFilter* filter, bool isComposite) 
{
   filters.push_back(ValueType(filter, isComposite));
}

////////////////////////////////////////////////////
bool AndCompositeTestCaseFilter::
isCaseMatch(const TestCaseInfoReader* testcase) const
{
   return This->isCaseMatch(testcase);
}

////////////////////////////////////////////////////////
void AndCompositeTestCaseFilter::
addFilter(const TestCaseFilter* filter, bool isComposite) 
{
   return This->addFilter(filter, isComposite);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
