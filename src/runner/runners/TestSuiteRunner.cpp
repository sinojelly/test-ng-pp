
#include <testngpp/ExceptionKeywords.h>
#include <testngpp/Error.h>

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
   TestSuiteRunnerImpl( TestSuiteLoader* loader
                      , TestFixtureRunner* runner
                      , TestResultCollector* collector)
      : fixtureRunner(runner)
      , suiteLoader(loader)
      , resultCollector(collector)
   {}

	TestSuiteDesc* load
         ( const StringList& searchingPaths
         , const std::string& path);

	void runAllFixtures(TestSuiteDesc* desc, const TestFilter* filter);

	void run( const StringList& searchingPaths
           , const std::string& path
   		  , const TestFilter* filter);

   std::string getDescEntryName() const
   { return testngppTestSuiteDescGetter; }

   TestFixtureRunner* fixtureRunner;
   TestResultCollector* resultCollector;
   TestSuiteLoader* suiteLoader;
};

/////////////////////////////////////////////////////////////////
TestSuiteRunner::TestSuiteRunner
   ( TestSuiteLoader* loader
   , TestFixtureRunner* runner
   , TestResultCollector* collector)
	: This(new TestSuiteRunnerImpl(loader, runner, collector))
{
}

/////////////////////////////////////////////////////////////////
TestSuiteRunner::~TestSuiteRunner()
{
   delete This;
}

/////////////////////////////////////////////////////////////////
TestSuiteDesc*
TestSuiteRunnerImpl::load
   ( const StringList& searchingPaths
   , const std::string& path)
{
   __TESTNGPP_TRY
   {
      return suiteLoader->load(searchingPaths, path, this);
   }
   __TESTNGPP_CATCH(std::exception& e)
   {
      resultCollector->addError("test suite \"" + path + "\" can't be loaded : " + e.what());
   }
   __TESTNGPP_END_TRY

   return 0;
}

/////////////////////////////////////////////////////////////////
void
TestSuiteRunnerImpl::runAllFixtures(TestSuiteDesc* desc
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
TestSuiteRunnerImpl::run
   ( const StringList& searchingPaths
   , const std::string& path
   , const TestFilter* filter)
{
   TestSuiteDesc* desc = load(searchingPaths, path);
   if(desc == 0)
   {
      return;
   }

   resultCollector->startTestSuite(desc);
	runAllFixtures(desc, filter);
   resultCollector->endTestSuite(desc);

   suiteLoader->unload();
}

/////////////////////////////////////////////////////////////////
void
TestSuiteRunner::run
   ( const StringList& searchingPaths
   , const std::string& path
   , const TestFilter* filter)
{
   This->run(searchingPaths, path, filter);
}

/////////////////////////////////////////////////////////////////

TESTNGPP_NS_END

