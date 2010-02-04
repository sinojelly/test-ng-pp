
#include <testngpp/internal/TestSuiteDesc.h>

#include <testngpp/runner/TestSuiteRunner.h>
#include <testngpp/runner/TestFilter.h>
#include <testngpp/runner/TestFixtureRunner.h>
#include <testngpp/runner/TestResultCollector.h>
#include <testngpp/runner/TestSuiteContext.h>

TESTNGPP_NS_START

/////////////////////////////////////////////////////////////////
struct TestSuiteRunnerImpl 
{
   TestSuiteRunnerImpl( TestFixtureRunner* runner
                      , TestResultCollector* collector)
      : fixtureRunner(runner)
      , resultCollector(collector)
   {}

   ~TestSuiteRunnerImpl()
   {
   }

	void runAllFixtures();

	void run(TestSuiteContext* suite);

   TestFixtureRunner* fixtureRunner;     // X
   TestResultCollector* resultCollector; // X
};

/////////////////////////////////////////////////////////////////
TestSuiteRunner::TestSuiteRunner
   ( TestFixtureRunner* runner
   , TestResultCollector* collector)
	: This(new TestSuiteRunnerImpl(runner, collector))
{
}

/////////////////////////////////////////////////////////////////
TestSuiteRunner::~TestSuiteRunner()
{
   delete This;
}

/////////////////////////////////////////////////////////////////
void
TestSuiteRunnerImpl::runAllFixtures()
{
#if 0
   for(unsigned int i=0; i<desc->getNumberOfTestFixtures(); i++)
   {
      TestFixtureDesc* fixture = desc->getTestFixture(i);
      if(filter->isFixtureMatch((const TestFixtureInfoReader*)fixture))
      {
         fixtureRunner->run(fixture, resultCollector, filter);
      }
   }
#endif
}

/////////////////////////////////////////////////////////////////
void
TestSuiteRunnerImpl::
run(TestSuiteContext* suite)
{
#if 0
   resultCollector->startTestSuite(desc);
	runAllFixtures(desc, filter);
   resultCollector->endTestSuite(desc);
#endif

}

/////////////////////////////////////////////////////////////////
void
TestSuiteRunner::
run(TestSuiteContext* suite)
{
   This->run(suite);
}

/////////////////////////////////////////////////////////////////

TESTNGPP_NS_END

