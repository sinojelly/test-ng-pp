
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/internal/TestSuiteDesc.h>

#include <testngpp/runner/TestSuiteRunner.h>
#include <testngpp/runner/TestFilter.h>
#include <testngpp/runner/TestSuiteLoader.h>
#include <testngpp/runner/TestFixtureRunner.h>
#include <testngpp/runner/TestResultCollector.h>
#include <testngpp/runner/TestSuiteDescEntryNameGetter.h>

TESTNGPP_NS_START

/////////////////////////////////////////////////////////////////
namespace
{
   const std::string testngppTestSuiteDescGetter("___testngpp_test_suite_desc_getter");
}

/////////////////////////////////////////////////////////////////
struct TestSuiteRunnerImpl : public TestSuiteDescEntryNameGetter
{
   TestSuiteRunnerImpl(TestFixtureRunner* runner, TestSuiteLoader* loader)
		: fixtureRunner(runner), suiteLoader(loader)
   {}

	TestSuiteDesc* load(const std::string& path
         , TestResultCollector* resultCollector);

	void runAllFixtures(TestSuiteDesc* desc
   		, TestResultCollector* resultCollector
         , const TestFilter* filter);

	void runAllFixtures(TestSuiteDesc* desc, const TestFilter* filter);

	void run( const std::string& path
           , TestResultCollector* resultCollector
   		  , const TestFilter* filter);

   std::string getDescEntryName() const
   { return testngppTestSuiteDescGetter; }

   TestFixtureRunner* fixtureRunner;
   TestSuiteLoader* suiteLoader;
};

/////////////////////////////////////////////////////////////////
TestSuiteRunner::TestSuiteRunner(TestSuiteLoader* loader
   , TestFixtureRunner* runner)
	: This(new TestSuiteRunnerImpl(runner, loader))
{
}

/////////////////////////////////////////////////////////////////
TestSuiteRunner::~TestSuiteRunner()
{
   delete This;
}

/////////////////////////////////////////////////////////////////
TestSuiteDesc*
TestSuiteRunnerImpl::load(const std::string& path
   , TestResultCollector* resultCollector)
{
   __TESTNGPP_TRY
 //  try
   {
     return suiteLoader->load(path, this);
   }
   __TESTNGPP_CATCH(std::exception& e)
   //catch(std::exception& e)
   {
      resultCollector->addError("test suite \"" + path + "\" can't be loaded : " + e.what());
   }
   __TESTNGPP_END_TRY

   return 0;
}

/////////////////////////////////////////////////////////////////
void
TestSuiteRunnerImpl::runAllFixtures(TestSuiteDesc* desc
   , TestResultCollector* resultCollector
   , const TestFilter* filter)
{
   for(unsigned int i=0; i<desc->getNumberOfTestFixtures(); i++)
   {
      TestFixtureDesc* fixture = desc->getTestFixture(i);
      if(filter->isFixtureMatch((const TestFixtureInfoReader*)fixture))
      {
         fixtureRunner->run(fixture, resultCollector, filter);
      }
   }
}

/////////////////////////////////////////////////////////////////
void
TestSuiteRunnerImpl::run(const std::string& path
   , TestResultCollector* resultCollector
   , const TestFilter* filter)
{
   TestSuiteDesc* desc = load(path, resultCollector);
   if(desc == 0)
   {
      return;
   }

   resultCollector->startTestSuite(desc);
	runAllFixtures(desc, resultCollector, filter);
   resultCollector->endTestSuite(desc);

   suiteLoader->unload();
}

/////////////////////////////////////////////////////////////////
void
TestSuiteRunner::run(const std::string& path
   , TestResultCollector* resultCollector
   , const TestFilter* filter)
{
   This->run(path, resultCollector, filter);
}

/////////////////////////////////////////////////////////////////

TESTNGPP_NS_END

