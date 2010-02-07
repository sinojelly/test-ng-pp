
#ifndef __TESTNGPP_EMPTY_TAG_FILTER_H
#define __TESTNGPP_EMPTY_TAG_FILTER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/TaggableObjFilter.h>

TESTNGPP_NS_START

struct EmptyTagFilter
   : public TaggableObjFilter
{
   bool matches(const Taggable* ) const; 
   void dump() const;
};

TESTNGPP_NS_END

#endif
