
#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/SimpleTestHierarchyRunner.h>
#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/runner/TestCaseRunner.h>
#include <testngpp/runner/TestFixtureResultCollector.h>
#include <testngpp/runner/TestCaseFilter.h>

TESTNGPP_NS_START

///////////////////////////////////////////////////////
struct SimpleTestHierarchyRunnerImpl 
{
	SimpleTestHierarchyRunnerImpl(TestCaseRunner* runner)
      : caseRunner(runner)
   {}

	void run(TestCaseHierarchy* hierarchy
      , TestFixtureResultCollector* resultCollector);

   TestCaseRunner* caseRunner;
};

///////////////////////////////////////////////////////
void
SimpleTestHierarchyRunnerImpl::run(TestCaseHierarchy* fixture
        , TestFixtureResultCollector* resultCollector)
{
#if 0
   unsigned int numberOfTestCases = fixture->getNumberOfTestCases();
   for(unsigned int i=0; i<numberOfTestCases; i++)
   {
      TestCase* testcase = fixture->getTestCase(i);
      if(filter->isCaseMatch((const TestCaseInfoReader*)testcase))
      {
         caseRunner->run(testcase, resultCollector);
      }
   }
#endif
}

///////////////////////////////////////////////////////
SimpleTestHierarchyRunner::
SimpleTestHierarchyRunner(TestCaseRunner* caseRunner)
   : This(new SimpleTestHierarchyRunnerImpl(caseRunner))
{
}

///////////////////////////////////////////////////////
SimpleTestHierarchyRunner::~SimpleTestHierarchyRunner()
{
   delete This;
}

///////////////////////////////////////////////////////
void
SimpleTestHierarchyRunner::run(TestCaseHierarchy* hierarchy
      , TestFixtureResultCollector* resultCollector)
{
   This->run(hierarchy, resultCollector);
}

///////////////////////////////////////////////////////

TESTNGPP_NS_END

