
#include <vector>
#include <iostream>

#include <testngpp/runner/AndCompositeTaggableFilter.h>
#include <testngpp/runner/OrCompositeTaggableFilter.h>
#include <testngpp/runner/NotCompositeTaggableFilter.h>

#include <testngpp/runner/TaggableObjFilter.h>
#include <testngpp/runner/TagsFilters.h>

#include <testngpp/internal/Taggable.h>

#include <testngpp/Error.h>


TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct TagsFiltersImpl
{
   TagsFiltersImpl();
   ~TagsFiltersImpl();

   void addNextFilter(TaggableObjFilter*);
   bool startOnNext();
   bool shouldRun(const Taggable* obj) const;
   bool shouldReport(const Taggable* obj) const;
   bool shouldStore(const Taggable* obj) const;
   bool hasBeenRunAlready(const Taggable* obj) const;

   std::vector<TaggableObjFilter*> filters;
   OrCompositeTaggableFilter    allTagsFilter;
   OrCompositeTaggableFilter    doneTagsFilter; 
   NotCompositeTaggableFilter   notDoneFilter;
   int index;
};

////////////////////////////////////////////////////////
TagsFiltersImpl::
TagsFiltersImpl()
   : notDoneFilter(&doneTagsFilter, false)
   , index(-1)
{
}

////////////////////////////////////////////////////////
TagsFiltersImpl::
~TagsFiltersImpl()
{
   for(int i=0; i<filters.size(); i++)
   {
     delete filters[i];
   }

   filters.clear();
}

////////////////////////////////////////////////////////
void
TagsFiltersImpl::
addNextFilter(TaggableObjFilter* filter)
{
   filters.push_back(filter);
   allTagsFilter.addFilter(filter, false);
}

////////////////////////////////////////////////////////
bool
TagsFiltersImpl::
startOnNext() 
{
   index++;

   if(index >= filters.size())
   {
      return false;
   }

   if(index > 0)
   {
     doneTagsFilter.addFilter(filters[index-1], false);
   }

   return true;
}

////////////////////////////////////////////////////////
bool
TagsFiltersImpl::
shouldRun(const Taggable* obj) const
{
   if(index >= filters.size())
   {
      throw Error("internal error");
   }
   
   return filters[index]->matches(obj) &&
          notDoneFilter.matches(obj);
}

////////////////////////////////////////////////////////
bool
TagsFiltersImpl::
shouldReport(const Taggable* obj) const
{
   return allTagsFilter.matches(obj);
}

////////////////////////////////////////////////////////
bool
TagsFiltersImpl::
shouldStore(const Taggable* obj) const
{
   return !(filters[index]->matches(obj) ||
          doneTagsFilter.matches(obj) ||
          allTagsFilter.matches(obj));
}

////////////////////////////////////////////////////////
bool
TagsFiltersImpl::
hasBeenRunAlready(const Taggable* obj) const
{
   return doneTagsFilter.matches(obj);
}

////////////////////////////////////////////////////////
TagsFilters::
TagsFilters()
   : This(new TagsFiltersImpl())
{
}

////////////////////////////////////////////////////////
TagsFilters::
~TagsFilters()
{
   delete This;
}

////////////////////////////////////////////////////////
void
TagsFilters::
addNextFilter(TaggableObjFilter* filter)
{
   This->addNextFilter(filter);
}

////////////////////////////////////////////////////////
bool
TagsFilters::
startOnNext() 
{
   return This->startOnNext();
}

////////////////////////////////////////////////////////
bool
TagsFilters::
shouldRun(const Taggable* obj) const
{
   return This->shouldRun(obj);
}

////////////////////////////////////////////////////////
bool
TagsFilters::
shouldReport(const Taggable* obj) const
{
   return This->shouldReport(obj);
}

////////////////////////////////////////////////////////
bool
TagsFilters::
shouldStore(const Taggable* obj) const
{
   return This->shouldStore(obj);
}

////////////////////////////////////////////////////////
bool
TagsFilters::
hasBeenRunAlready(const Taggable* obj) const
{
   return This->hasBeenRunAlready(obj);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
