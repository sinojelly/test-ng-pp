
#ifndef __TESTNGPPST_TEST_RESULT_COLLECTOR_H
#define __TESTNGPPST_TEST_RESULT_COLLECTOR_H

#include <string>

#include <testngppst/testngppst.h>

#include <testngppst/listener/TagsFilterResultCollector.h>

TESTNGPPST_NS_START

struct TestResultCollector
   : public TagsFilterResultCollector
{
   virtual void startTest() = 0;
   virtual void endTest() = 0;

   virtual void addError(const std::string&) = 0;

	virtual ~TestResultCollector() {}
};

TESTNGPPST_NS_END

#endif

