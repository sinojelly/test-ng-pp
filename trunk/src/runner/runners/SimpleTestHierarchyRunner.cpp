
#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/SimpleTestHierarchyRunner.h>
#include <testngpp/runner/TestHierarchyHandler.h>
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

	void run(TestHierarchyHandler* hierarchy
      , TestFixtureResultCollector* resultCollector);

   TestCaseRunner* caseRunner;
};

///////////////////////////////////////////////////////
void
SimpleTestHierarchyRunnerImpl::run(TestHierarchyHandler* hierarchy
        , TestFixtureResultCollector* resultCollector)
{
   while(hierarchy->numberOfTestCasesInSched() > 0)
   {
      TestCase* testcase = const_cast<TestCase*>(hierarchy->getTestCase(0));
      bool result = caseRunner->run(testcase, resultCollector);
      hierarchy->testDone(testcase, result);
   }
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
SimpleTestHierarchyRunner::run(TestHierarchyHandler* hierarchy
      , TestFixtureResultCollector* resultCollector)
{
   This->run(hierarchy, resultCollector);
}

///////////////////////////////////////////////////////

TESTNGPP_NS_END

