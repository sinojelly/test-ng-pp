
#include <utility>
#include <list>
#include <iostream>

#include <testngpp/internal/TestCase.h>

#include <testngpp/internal/TestFixtureDesc.h>

#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/runner/TestCaseContainer.h>
#include <testngpp/runner/TestHierarchyHandler.h>
#include <testngpp/runner/TestFixtureResultCollector.h>
#include <testngpp/runner/InternalError.h>


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
      if(!userSpecified)
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
         ( TestFixtureDesc* fixture 
         , const TestCaseFilter* filter
         , TestFixtureResultCollector* collector);

   ~TestHierarchyHandlerImpl();

   void addTestCase
         ( const TestCase* testcase
         , bool userSpecified);

   void testDone
         ( const TestCase* testcase
         , bool hasSucceeded);

   unsigned int numberOfTestCasesInSched() const
   { return schedTestCases.size(); }
 
   const TestCase* getTestCase(unsigned int index) const;

private:
   void handleDoneTestCases
         ( const TestCase* testcase
         , bool hasSucceeded);

private:
   typedef std::pair<const TestCase*, bool> ValueType;
   std::list<ValueType> schedTestCases;

   TestCaseHierarchy* hierarchy; // Y
   TestFixtureResultCollector* collector; // X
};


///////////////////////////////////////////////////
TestHierarchyHandlerImpl::
TestHierarchyHandlerImpl
   ( TestFixtureDesc* fixtureDesc 
   , const TestCaseFilter* filter
   , TestFixtureResultCollector* resultCollector)
   : hierarchy(new TestCaseHierarchy(fixtureDesc, filter))
   , collector(resultCollector)
{
   hierarchy->getDirectSuccessors(0, this);
}

///////////////////////////////////////////////////
TestHierarchyHandlerImpl::
~TestHierarchyHandlerImpl()
{
   delete hierarchy;
}

///////////////////////////////////////////////////
const TestCase*
TestHierarchyHandlerImpl::
getTestCase(unsigned int index) const
{
   if(index >= schedTestCases.size())
   {
      TESTNGPP_INTERNAL_ERROR(2010);
   }

   std::list<ValueType>::const_iterator iter = \
      schedTestCases.begin();

   for(int i=0; i < index; i++)
      iter++;

   return (*iter).first;
}

///////////////////////////////////////////////////
void
TestHierarchyHandlerImpl::
addTestCase
   ( const TestCase* testcase
   , bool userSpecified)
{
   schedTestCases.push_back(ValueType(testcase, userSpecified));
}

///////////////////////////////////////////////////
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
      hierarchy->getSuccessors(testcase, &skippedTestCases);
   }

}

///////////////////////////////////////////////////
void
TestHierarchyHandlerImpl::
testDone
   ( const TestCase* testcase
   , bool hasSucceeded)
{
   std::list<ValueType>::iterator i = schedTestCases.begin();
   for(; i != schedTestCases.end(); i++)
   {
      if((*i).first == testcase)
      {
         schedTestCases.erase(i);
         handleDoneTestCases(testcase, hasSucceeded);
         return;
      }
   }
}

///////////////////////////////////////////////////
TestHierarchyHandler::
TestHierarchyHandler
   ( TestFixtureDesc* fixtureDesc 
   , const TestCaseFilter*  filter
   , TestFixtureResultCollector* collector)
   : This(new TestHierarchyHandlerImpl(fixtureDesc, filter, collector))
{
}

///////////////////////////////////////////////////
TestHierarchyHandler::
~TestHierarchyHandler()
{
   delete This;
}

///////////////////////////////////////////////////
void
TestHierarchyHandler::
testDone(const TestCase* testcase, bool hasSucceeded)
{
   return This->testDone(testcase, hasSucceeded);
}

///////////////////////////////////////////////////
unsigned int
TestHierarchyHandler::
numberOfTestCasesInSched() const
{
   return This->numberOfTestCasesInSched();
}

///////////////////////////////////////////////////
const TestCase*
TestHierarchyHandler::
getTestCase(unsigned int index) const
{
   return This->getTestCase(index);
}

///////////////////////////////////////////////////

TESTNGPP_NS_END

