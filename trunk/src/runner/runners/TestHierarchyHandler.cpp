
#include <list>

#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/runner/TestCaseContainer.h>
#include <testngpp/runner/TestHierarchyHandler.h>

TESTNGPP_NS_START

///////////////////////////////////////////////////
struct TestHierarchyHandlerImpl
   : public TestCaseContainer
{
   TestHierarchyHandlerImpl
         ( TestCaseHierarchy* hierarchy
         , TestFixtureResultCollector* collector);

   ~TestHierarchyHandlerImpl();

   void addTestCase
         ( TestCase* testcase
         , bool useSpecified);

   void testDone
         ( const TestCase* testcase
         , bool hasSucceeded);

   unsigned int numberOfTestCasesInSched() const;
 
   TestCase* getTestCase(unsigned int index) const;

private:

   TestCaseHierarchy* hierarchy;
   TestFixtureResultCollector* collector;
};


///////////////////////////////////////////////////
TestHierarchyHandlerImpl::
TestHierarchyHandlerImpl
   ( TestCaseHierarchy* testHierarchy
   , TestFixtureResultCollector* resultCollector)
   : hierarchy(testHierarchy)
   , collector(resultCollector)
{
}

///////////////////////////////////////////////////
TestHierarchyHandlerImpl::
~TestHierarchyHandlerImpl()
{
}

///////////////////////////////////////////////////

TESTNGPP_NS_END

