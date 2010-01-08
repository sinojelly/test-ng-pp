
#include <testngpp/Error.h>

#include <testngpp/internal/TestFixtureInfoReader.h>
#include <testngpp/internal/TestFixtureDesc.h>

#include <testngpp/runner/TestFixtureRunnerWrapper.h>
#include <testngpp/runner/TestFixtureRunner.h>
#include <testngpp/runner/TestFixtureResultCollector.h>


TESTNGPP_NS_START

/////////////////////////////////////////////////////////////////
TestFixtureRunnerWrapper::TestFixtureRunnerWrapper(TestFixtureRunner* runner)
	: fixtureRunner(runner)
{
}

/////////////////////////////////////////////////////////////////
TestFixtureRunnerWrapper::~TestFixtureRunnerWrapper()
{
    delete fixtureRunner;
}

/////////////////////////////////////////////////////////////////
void TestFixtureRunnerWrapper::run(TestFixtureDesc* desc
     , TestFixtureResultCollector* collector
     , const TestCaseFilter* filter)
{
   collector->startTestFixture(desc);

   try{
      fixtureRunner->run(desc, collector, filter);
   }
   catch(Error& e)
   {
      collector->addFixtureError(desc, e.what());
   }
   catch(...)
   {
      collector->addFixtureError(desc, "Unknow Error");
   }

   collector->endTestFixture(desc);
}

TESTNGPP_NS_END

