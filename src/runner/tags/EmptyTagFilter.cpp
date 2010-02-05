
#include <testngpp/runner/EmptyTagFilter.h>
#include <testngpp/internal/Taggable.h>

TESTNGPP_NS_START

bool
EmptyTagFilter::
matches(const Taggable* obj) const
{
   return !obj->hasTags();
}

TESTNGPP_NS_END


