
#include <testngpp/runner/TaggableObjFilter.h>
#include <testngpp/runner/GeneralTagsFilter.h>
#include <testngpp/internal/Taggable.h>
#include <testngpp/internal/NameMatcher.h>
#include <testngpp/runner/InternalError.h>

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
matches(const Taggable* obj) const
{
   if(obj == 0 || This->tagsMatcher)
   {
      TESTNGPP_INTERNAL_ERROR(2016);
   }
      
   return obj->tagsMatch(This->tagsMatcher);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
