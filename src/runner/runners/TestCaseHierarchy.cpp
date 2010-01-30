
#include <list>

#include <testngpp/internal/TestFixtureDesc.h>
#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/runner/TestCaseFilter.h>
#include <testngpp/runner/TestCaseRunner.h>

TESTNGPP_NS_START

namespace {

struct Hierarchy
{
   Hierarchy(TestCase* rootCase)
      : root(rootCase)
      , specified(false)
   {}

   void addTestCase(TestCase* testcase);

private:
   typedef std::list<Hierarchy> Successors;
   typedef Successors::iterator Iterator;

private:
   TestCase*  root;
   bool       specified;
   Successors successors;

private:
   Hierarchy& addDirectSuccessor(TestCase* testcase);

   bool addSuccessor(TestCase* testcase);

   void run(TestCaseRunner* runner);
};

///////////////////////////////////////////////////////
Hierarchy&
Hierarchy::addDirectSuccessor(TestCase* testcase)
{
   Iterator i = successors.begin();
   for(; i != successors.end(); i++)
   {
      if((*i).root == testcase)
         return (*i);
   }

   successors.push_back(Hierarchy(testcase));

   return successors.back();
}

///////////////////////////////////////////////////////
bool
Hierarchy::addSuccessor(TestCase* testcase)
{
   TestCase* test = testcase;

   while(test != 0)
   {
     TestCase* depends = test->getDependedTestCase();
     if(depends == root)
     {
        addDirectSuccessor(test).addTestCase(testcase);
        break;
     }
     test = depends;
   }
}

///////////////////////////////////////////////////////
void
Hierarchy::addTestCase(TestCase* testcase)
{
   if(root == testcase)
   {
      specified = true;
      return;
   }

   addSuccessor(testcase);
}

}

///////////////////////////////////////////////////////
struct TestCaseHierarchyImpl
{
   TestCaseHierarchyImpl
      ( const TestFixtureDesc * fixtureDesc 
      , const TestCaseFilter* filter);

   Hierarchy root;
};

///////////////////////////////////////////////////////
TestCaseHierarchyImpl::TestCaseHierarchyImpl
   ( const TestFixtureDesc * fixture
   , const TestCaseFilter* filter)
   : root(0)
{

   unsigned int numberOfTestCases = fixture->getNumberOfTestCases();
   for(unsigned int i=0; i<numberOfTestCases; i++)
   {
      TestCase* testcase = fixture->getTestCase(i);
      if(filter->isCaseMatch((const TestCaseInfoReader*)testcase))
      {
         root.addTestCase(testcase);
      }

   }
}

///////////////////////////////////////////////////////
TestCaseHierarchy::TestCaseHierarchy
   ( const TestFixtureDesc* fixture
   , const TestCaseFilter* filter)
   : This(new TestCaseHierarchyImpl(fixture, filter))
{
}

///////////////////////////////////////////////////////
TestCaseHierarchy::~TestCaseHierarchy()
{
   delete This;
}

///////////////////////////////////////////////////////

TESTNGPP_NS_END

