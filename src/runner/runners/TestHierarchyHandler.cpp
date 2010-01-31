
#include <utility>
#include <list>

#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/runner/TestCaseContainer.h>
#include <testngpp/runner/TestHierarchyHandler.h>
#include <testngpp/runner/TestFixtureResultCollector.h>

TESTNGPP_NS_START

namespace
{

struct SkippedTestCases
   : public TestCaseContainer
{
   SkippedTestCases
         ( TestFixtureResultCollector* resultCollector)
         : collector(resultCollector)
   {}

   void addTestCase
         ( const TestCase* testcase
         , bool userSpecified)
   {
      if(userSpecified)
         return;

      collector->startTestCase(testcase);
      collector->addCaseSkipped(testcase);
      collector->endTestCase(testcase);
   }

private:
   TestFixtureResultCollector* collector;
};

}
   
///////////////////////////////////////////////////
struct TestHierarchyHandlerImpl
   : public TestCaseContainer
{
   TestHierarchyHandlerImpl
         ( TestCaseHierarchy* hierarchy
         , TestFixtureResultCollector* collector);

   ~TestHierarchyHandlerImpl();

   void addTestCase
         ( const TestCase* testcase
         , bool userSpecified);

   void testDone
         ( const TestCase* testcase
         , bool hasSucceeded);

   unsigned int numberOfTestCasesInSched() const;
 
   TestCase* getTestCase(unsigned int index) const;

private:
   void handleDoneTestCases
         ( const TestCase* testcase
         , bool hasSucceeded);

private:
   typedef std::pair<const TestCase*, bool> ValueType;
   std::list<ValueType> shedTestCases;

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
void
TestHierarchyHandlerImpl::
addTestCase
   ( const TestCase* testcase
   , bool userSpecified)
{
   shedTestCases.push_back(ValueType(testcase, userSpecified));
}

void
TestHierarchyHandlerImpl::
handleDoneTestCases
   ( const TestCase* testcase
   , bool hasSucceeded)
{
   if(hasSucceeded)
   {
      hierarchy->getDirectSuccessors(testcase, this);
   }
   else
   {
      SkippedTestCases skippedTestCases(collector);
      hierarchy->getDirectSuccessors(testcase, &skippedTestCases);
   }

}

///////////////////////////////////////////////////
void
TestHierarchyHandlerImpl::
testDone
   ( const TestCase* testcase
   , bool hasSucceeded)
{
   std::list<ValueType>::iterator i = shedTestCases.begin();
   for(; i != shedTestCases.end(); i++)
   {
      if((*i).first == testcase)
      {
         shedTestCases.erase(i);
         handleDoneTestCases(testcase, hasSucceeded);
         return;
      }
   }
}

///////////////////////////////////////////////////

TESTNGPP_NS_END

