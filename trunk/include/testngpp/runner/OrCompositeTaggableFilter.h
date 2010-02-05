
#ifndef __TESTNGPP_OR_COMPOSITE_TAGGABLE_FILTER_H
#define __TESTNGPP_OR_COMPOSITE_TAGGABLE_FILTER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TaggableObjFilter.h>

TESTNGPP_NS_START

struct Taggable;
struct OrCompositeTaggableFilterImpl;

////////////////////////////////////////////////////////
struct OrCompositeTaggableFilter
   : public TaggableObjFilter
{
   OrCompositeTaggableFilter();
   ~OrCompositeTaggableFilter();

   void addFilter(const TaggableObjFilter* filter, bool isComposite=true);
   bool matches(const Taggable* ) const;

private:
   OrCompositeTaggableFilterImpl * This;

};

////////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

