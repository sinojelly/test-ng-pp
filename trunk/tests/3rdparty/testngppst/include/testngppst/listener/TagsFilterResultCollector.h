
#ifndef __TESTNGPPST_TAGS_FILTER_RESULT_COLLECTOR_H
#define __TESTNGPPST_TAGS_FILTER_RESULT_COLLECTOR_H

#include <string>

#include <testngppst/testngppst.h>

#include <testngppst/listener/TestSuiteResultCollector.h>

TESTNGPPST_NS_START

struct TagsFilterRule;

struct TagsFilterResultCollector
   : public TestSuiteResultCollector
{
   virtual void startTagsFiltering(const TagsFilterRule*) = 0;
   virtual void endTagsFiltering(const TagsFilterRule*) = 0;

   virtual ~TagsFilterResultCollector() {}
};

TESTNGPPST_NS_END

#endif

