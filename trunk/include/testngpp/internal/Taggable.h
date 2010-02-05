#ifndef __TESTNGPP_TAGGABLE_H
#define __TESTNGPP_TAGGABLE_H

#include <string>

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct NameMatcher;

////////////////////////////////////////////////////
struct Taggable
{
   virtual
   bool hasTags() const = 0;

   virtual
   bool tagsMatch(const NameMatcher* matcher) const = 0;

   virtual ~Taggable() {}
};

////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

