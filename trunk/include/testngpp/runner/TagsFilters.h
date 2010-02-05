
#ifndef __TESTNGPP_TAGS_FILTERS_H
#define __TESTNGPP_TAGS_FILTERS_H

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct Taggable;
struct TaggableObjFilter;
struct TagsFiltersImpl;

////////////////////////////////////////////////////////
struct TagsFilters
{
   TagsFilters();
   ~TagsFilters();

   void addNextFilter(TaggableObjFilter*);

   bool startOnNext();

   bool shouldRun(const Taggable* ) const;
   bool shouldReport(const Taggable* ) const;
   bool shouldStore(const Taggable* ) const;
   bool hasBeenRunAlready(const Taggable* ) const;

private:
   TagsFiltersImpl * This;
};

////////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

