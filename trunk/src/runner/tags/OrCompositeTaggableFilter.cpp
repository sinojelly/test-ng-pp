
#include <list>
#include <algorithm>
#include <iostream>

#include <testngpp/runner/TaggableObjFilter.h>
#include <testngpp/runner/OrCompositeTaggableFilter.h>
#include <testngpp/internal/Taggable.h>
#include <testngpp/runner/InternalError.h>

#include <testngpp/runner/AndCompositeTaggableFilter.h>
#include <testngpp/runner/NotCompositeTaggableFilter.h>

#include <testngpp/runner/GeneralTagsFilter.h>
#include <testngpp/runner/EmptyTagFilter.h>

TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct OrCompositeTaggableFilterImpl
{
   typedef std::pair<const TaggableObjFilter*, bool> ValueType;
   typedef std::list<ValueType> Filters;

   Filters filters;

   ~OrCompositeTaggableFilterImpl();

   void addFilter(const TaggableObjFilter* filter, bool isComposite);
   bool matches(const Taggable* obj) const;
};

////////////////////////////////////////////////////////
OrCompositeTaggableFilterImpl::
~OrCompositeTaggableFilterImpl()
{
   Filters::iterator i = filters.begin();
   for(; i != filters.end(); i++)
   {
      if((*i).second)
         delete (*i).first;
   }
}

////////////////////////////////////////////////////////
OrCompositeTaggableFilter::
OrCompositeTaggableFilter()
   : This(new OrCompositeTaggableFilterImpl())
{
}

////////////////////////////////////////////////////////
OrCompositeTaggableFilter::
~OrCompositeTaggableFilter()
{
   delete This;
}

////////////////////////////////////////////////////
bool OrCompositeTaggableFilterImpl::
matches(const Taggable* obj) const
{
   if(obj == 0)
   {
      TESTNGPP_INTERNAL_ERROR(2011);
   }
   
   Filters::const_iterator i = filters.begin();
   for(; i != filters.end(); i++)
   {
      if((*i).first == 0)
      {
         TESTNGPP_INTERNAL_ERROR(2012);
      }
      
      if((*i).first->matches(obj))
      {
         return true;
      }
   }

   return false;
}

////////////////////////////////////////////////////
void OrCompositeTaggableFilterImpl::
addFilter(const TaggableObjFilter* filter, bool isComposite) 
{
   filters.push_back(ValueType(filter, isComposite));
}

////////////////////////////////////////////////////
bool OrCompositeTaggableFilter::
matches(const Taggable* obj) const
{
   return This->matches(obj);
}

////////////////////////////////////////////////////////
void OrCompositeTaggableFilter::
addFilter(const TaggableObjFilter* filter, bool isComposite) 
{
   return This->addFilter(filter, isComposite);
}

////////////////////////////////////////////////////////
bool OrCompositeTaggableFilter::
isEmpty() const
{
   return This->filters.size() == 0;
}

////////////////////////////////////////////////////////
void OrCompositeTaggableFilter::
dump() const
{
   std::cout << "(";
   
   OrCompositeTaggableFilterImpl::Filters::const_iterator i = This->filters.begin();
   for(; i != This->filters.end(); i++)
   {
      (*i).first->dump();
   }
   
   std::cout << ")";
   
   std::cout.flush();
}

TESTNGPP_NS_END
