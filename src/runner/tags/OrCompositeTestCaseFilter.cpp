
#include <list>
#include <algorithm>

#include <testngpp/runner/OrCompositeTestCaseFilter.h>
#include <testngpp/internal/TestCase.h>

TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct OrCompositeTestCaseFilterImpl
{
   typedef std::pair<const TestCaseFilter*, bool> ValueType;
   typedef std::list<ValueType> Filters;

   Filters filters;

   ~OrCompositeTestCaseFilterImpl();

   void addFilter(const TestCaseFilter* filter, bool isComposite);
   bool isCaseMatch(const TestCase* testcase) const;
};

////////////////////////////////////////////////////////
OrCompositeTestCaseFilterImpl::
~OrCompositeTestCaseFilterImpl()
{
   Filters::iterator i = filters.begin();
   for(; i != filters.end(); i++)
   {
      if((*i).second)
         delete (*i).first;
   }
}

////////////////////////////////////////////////////////
OrCompositeTestCaseFilter::
OrCompositeTestCaseFilter()
   : This(new OrCompositeTestCaseFilterImpl())
{
}

////////////////////////////////////////////////////////
OrCompositeTestCaseFilter::
~OrCompositeTestCaseFilter()
{
   delete This;
}

////////////////////////////////////////////////////
bool OrCompositeTestCaseFilterImpl::
isCaseMatch(const TestCase* testcase) const
{
   Filters::const_iterator i = filters.begin();
   for(; i != filters.end(); i++)
   {
      if((*i).first->isCaseMatch(testcase))
         return true;
   }

   return false;
}

////////////////////////////////////////////////////
void OrCompositeTestCaseFilterImpl::
addFilter(const TestCaseFilter* filter, bool isComposite) 
{
   filters.push_back(ValueType(filter, isComposite));
}

////////////////////////////////////////////////////
bool OrCompositeTestCaseFilter::
isCaseMatch(const TestCase* testcase) const
{
   return This->isCaseMatch(testcase);
}

////////////////////////////////////////////////////////
void OrCompositeTestCaseFilter::
addFilter(const TestCaseFilter* filter, bool isComposite) 
{
   return This->addFilter(filter, isComposite);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
