
#ifndef __TESTNGPP_TEST_HIERARCHY_HANDLER_H
#define __TESTNGPP_TEST_HIERARCHY_HANDLER_H

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestFixtureDesc;
struct TestCaseFilter;
struct FixtureTagsFilter;
struct TestFixtureResultCollector;
struct TestHierarchyHandlerImpl;

struct TestHierarchyHandler
{
   TestHierarchyHandler
         ( TestFixtureDesc* fixture
         , const TestCaseFilter* filter
         , FixtureTagsFilter* tagsFilter
         , TestFixtureResultCollector* collector);

   ~TestHierarchyHandler();

   void testDone(const TestCase* testcase, bool hasSucceeded);

   unsigned int numberOfTestCasesInSched() const;
 
   const TestCase* getTestCase(unsigned int index) const;

private:
   TestHierarchyHandlerImpl * This;
};

TESTNGPP_NS_END

#endif

