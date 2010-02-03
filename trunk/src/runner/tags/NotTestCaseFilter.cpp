
#include <testngpp/runner/NotTestCaseFilter.h>
#include <testngpp/internal/TestCase.h>

TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct NotTestCaseFilterImpl
{
   TestCaseFilter* filter;
   bool isComposite;

   NotTestCaseFilterImpl(TestCaseFilter*, bool);
   ~NotTestCaseFilterImpl();

   bool isCaseMatch(const TestCase* testcase) const;
};

////////////////////////////////////////////////////////
NotTestCaseFilterImpl::
NotTestCaseFilterImpl(TestCaseFilter* tagsFilter, bool composite)
   : filter(tagsFilter)
   , isComposite(composite)
{
}

////////////////////////////////////////////////////////
NotTestCaseFilterImpl::
~NotTestCaseFilterImpl()
{
   if(isComposite && filter != 0)
      delete filter;
}

////////////////////////////////////////////////////////
NotTestCaseFilter::
NotTestCaseFilter(TestCaseFilter* filter, bool composite)
   : This(new NotTestCaseFilterImpl(filter, composite))
{
}

////////////////////////////////////////////////////////
NotTestCaseFilter::
~NotTestCaseFilter()
{
   delete This;
}

////////////////////////////////////////////////////
bool NotTestCaseFilterImpl::
isCaseMatch(const TestCase* testcase) const
{
   if(filter == 0)
     return true;

   return !filter->isCaseMatch(testcase);
}

////////////////////////////////////////////////////
bool NotTestCaseFilter::
isCaseMatch(const TestCase* testcase) const
{
   return This->isCaseMatch(testcase);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
