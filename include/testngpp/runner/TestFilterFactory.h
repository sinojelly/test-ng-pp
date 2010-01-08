#ifndef __TESTNGPP_TEST_FILTER_FACTORY_H
#define __TESTNGPP_TEST_FILTER_FACTORY_H

#include <list>
#include <string>

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestFilter;

////////////////////////////////////////////////////
struct TestFilterFactory
{
   static const TestFilter* getFilter(const std::list<std::string>& filterOptions);
   static void returnFilter(const TestFilter* filter);
};

////////////////////////////////////////////////////

TESTNGPP_NS_END

#endif

