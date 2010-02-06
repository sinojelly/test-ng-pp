
#include <testngpp/runner/EmptyTagFilter.h>
#include <testngpp/internal/Taggable.h>
#include <testngpp/runner/InternalError.h>

TESTNGPP_NS_START

bool
EmptyTagFilter::
matches(const Taggable* obj) const
{
   if(obj == 0)
   {
      TESTNGPP_INTERNAL_ERROR(2016);
   }
      
   return !obj->hasTags();
}

TESTNGPP_NS_END


