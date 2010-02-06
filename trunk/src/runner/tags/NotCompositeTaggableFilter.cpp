
#include <testngpp/runner/NotCompositeTaggableFilter.h>
#include <testngpp/internal/Taggable.h>

TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct NotCompositeTaggableFilterImpl
{
   TaggableObjFilter* filter;
   bool isComposite;

   NotCompositeTaggableFilterImpl(TaggableObjFilter*, bool);
   ~NotCompositeTaggableFilterImpl();

   bool matches(const Taggable* obj) const;
};

////////////////////////////////////////////////////////
NotCompositeTaggableFilterImpl::
NotCompositeTaggableFilterImpl(TaggableObjFilter* tagsFilter, bool composite)
   : filter(tagsFilter)
   , isComposite(composite)
{
}

////////////////////////////////////////////////////////
NotCompositeTaggableFilterImpl::
~NotCompositeTaggableFilterImpl()
{
   if(isComposite && filter != 0)
      delete filter;
}

////////////////////////////////////////////////////////
NotCompositeTaggableFilter::
NotCompositeTaggableFilter(TaggableObjFilter* filter, bool composite)
   : This(new NotCompositeTaggableFilterImpl(filter, composite))
{
}

////////////////////////////////////////////////////////
NotCompositeTaggableFilter::
~NotCompositeTaggableFilter()
{
   delete This;
}

////////////////////////////////////////////////////
bool NotCompositeTaggableFilterImpl::
matches(const Taggable* obj) const
{   
   if(filter == 0)
     return true;

   return !filter->matches(obj);
}

////////////////////////////////////////////////////
bool NotCompositeTaggableFilter::
matches(const Taggable* obj) const
{
   return This->matches(obj);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
