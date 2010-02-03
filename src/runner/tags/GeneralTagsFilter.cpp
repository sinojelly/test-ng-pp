
#include <testngpp/runner/TestCaseFilter.h>
#include <testngpp/runner/GeneralTagsFilter.h>
#include <testngpp/internal/TestCase.h>
#include <testngpp/internal/NameMatcher.h>

TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct GeneralTagsFilterImpl
{
   GeneralTagsFilterImpl(NameMatcher* matcher);

   ~GeneralTagsFilterImpl();
   NameMatcher* tagsMatcher;
};

////////////////////////////////////////////////////////
GeneralTagsFilterImpl::
GeneralTagsFilterImpl(NameMatcher* matcher)
   : tagsMatcher(matcher)
{
}
////////////////////////////////////////////////////////
GeneralTagsFilterImpl::~GeneralTagsFilterImpl()
{
   delete tagsMatcher;
}
////////////////////////////////////////////////////////
GeneralTagsFilter::
GeneralTagsFilter(NameMatcher* matcher)
   : This(new GeneralTagsFilterImpl(matcher))
{
}

////////////////////////////////////////////////////////
GeneralTagsFilter::~GeneralTagsFilter()
{
   delete This;
}

////////////////////////////////////////////////////
bool GeneralTagsFilter::
isCaseMatch(const TestCase* testcase) const
{
   return testcase->tagsMatch(This->tagsMatcher);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
