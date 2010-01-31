
#include <list>

#include <testngpp/internal/TestFixtureDesc.h>
#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/runner/TestCaseFilter.h>
#include <testngpp/runner/TestCaseRunner.h>
#include <testngpp/runner/TestCaseContainer.h>

TESTNGPP_NS_START

namespace {

struct Hierarchy
{
   Hierarchy(const TestCase* rootCase)
      : root(rootCase)
      , specified(false)
   {}

   void addTestCase(const TestCase* testcase);

private:
   typedef std::list<Hierarchy> Successors;
   typedef Successors::iterator Iterator;

private:
   const TestCase*  root;
   bool       specified;
   Successors successors;

private:
   Hierarchy&
   addDirectSuccessor
         ( const TestCase* testcase );

   bool
   addSuccessor
         ( const TestCase* testcase );

   void
   getSuccessors
         ( const TestCase* testcase
         , TestCaseContainer* container );

   void
   putDirectSuccessorsToContainer
         ( TestCaseContainer* container );
public:
   void
   getDirectSuccessors
         ( const TestCase* testcase
         , TestCaseContainer* container );
};

///////////////////////////////////////////////////////
void Hierarchy::
putDirectSuccessorsToContainer
      ( TestCaseContainer* container )
{
   Successors::iterator i = successors.begin();
   for(; i != successors.end(); i++)
   {
      container->addTestCase((*i).root, (*i).specified);
   }
}

///////////////////////////////////////////////////////
void Hierarchy::
getSuccessors
     ( const TestCase* testcase
     , TestCaseContainer* container)
{
   Successors::iterator i = successors.begin();
   for(; i != successors.end(); i++)
   {
      (*i).getDirectSuccessors(testcase, container);
   }
}

///////////////////////////////////////////////////////
void Hierarchy::
getDirectSuccessors
      ( const TestCase* testcase
      , TestCaseContainer* container)
{
   if(testcase == root)
   {
      putDirectSuccessorsToContainer(container);
      return;
   }

   getSuccessors(testcase, container);
}

///////////////////////////////////////////////////////
Hierarchy&
Hierarchy::
addDirectSuccessor
      ( const TestCase* testcase )
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
Hierarchy::
addSuccessor
      ( const TestCase* testcase )
{
   const TestCase* test = testcase;

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
Hierarchy::addTestCase
     ( const TestCase* testcase )
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
void TestCaseHierarchy::
getDirectSuccessors
      ( const TestCase* testcase
      , TestCaseContainer* container)
{
   This->root.getDirectSuccessors(testcase, container);
}

TESTNGPP_NS_END

