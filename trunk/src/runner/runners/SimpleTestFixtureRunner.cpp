
#include <testngpp/internal/TestFixtureDesc.h>
#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/SimpleTestFixtureRunner.h>
#include <testngpp/runner/TestCaseRunner.h>
#include <testngpp/runner/TestFixtureResultCollector.h>
#include <testngpp/runner/TestCaseFilter.h>

TESTNGPP_NS_START

///////////////////////////////////////////////////////
struct SimpleTestFixtureRunnerImpl 
{
	SimpleTestFixtureRunnerImpl(TestCaseRunner* runner)
      : caseRunner(runner)
   {}

	void run(TestFixtureDesc* fixture
      , TestFixtureResultCollector* resultCollector
      , const TestCaseFilter* filter);

   TestCaseRunner* caseRunner;
};

///////////////////////////////////////////////////////
void
SimpleTestFixtureRunnerImpl::run(TestFixtureDesc* fixture
        , TestFixtureResultCollector* resultCollector
        , const TestCaseFilter* filter)
{
   unsigned int numberOfTestCases = fixture->getNumberOfTestCases();
   for(unsigned int i=0; i<numberOfTestCases; i++)
   {
      TestCase* testcase = fixture->getTestCase(i);
      if(filter->isCaseMatch((const TestCaseInfoReader*)testcase))
      {
         caseRunner->run(testcase, resultCollector);
      }
   }
}

///////////////////////////////////////////////////////
SimpleTestFixtureRunner::SimpleTestFixtureRunner(TestCaseRunner* caseRunner)
   : This(new SimpleTestFixtureRunnerImpl(caseRunner))
{
}

///////////////////////////////////////////////////////
SimpleTestFixtureRunner::~SimpleTestFixtureRunner()
{
   delete This;
}

///////////////////////////////////////////////////////
void
SimpleTestFixtureRunner::run(TestFixtureDesc* fixture
      , TestFixtureResultCollector* resultCollector
      , const TestCaseFilter* filter)
{
   This->run(fixture, resultCollector, filter);
}

///////////////////////////////////////////////////////

TESTNGPP_NS_END

