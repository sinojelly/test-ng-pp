
#include <iostream>

#include <testngpp/Error.h>
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/runner/TestSuiteRunner.h>
#include <testngpp/runner/TestFixtureRunnerFactory.h>
#include <testngpp/runner/InternalError.h>
#include <testngpp/runner/TestFilter.h>
#include <testngpp/runner/TestFilterFactory.h>
#include <testngpp/runner/ModuleTestListenerLoaderFactory.h>
#include <testngpp/runner/ModuleTestSuiteLoaderFactory.h>
#include <testngpp/runner/SimpleTestResultManager.h>
#include <testngpp/runner/TestResultManager.h>

#include <testngpp/runner/TestRunner.h>

TESTNGPP_NS_START

struct TestRunnerImpl
{
   TestFixtureRunner * fixtureRunner;
   TestSuiteRunner * suiteRunner;

   TestResultManager *resultManager;

   bool hasFailures;

   TestRunnerImpl();
   ~TestRunnerImpl();
   
   void createSuiteRunner(bool useSandbox, unsigned int maxConcurrent);

   void runTestSuite
      (TestSuiteContext* suite);

   void runTests( const StringList& searchingPaths
                , const StringList& suites
                , const TestFilter* filter);

   void loadListeners( const StringList& searchingPaths
                     , const StringList& listeners);

   void loadSuites(const StringList& suites);
};

///////////////////////////////////////////////////////
TestRunnerImpl::TestRunnerImpl()
   : fixtureRunner(0)
   , suiteRunner(0)
   , hasFailures(false)
{
   resultManager = new SimpleTestResultManager(new ModuleTestListenerLoaderFactory());
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
TestRunnerImpl::~TestRunnerImpl()
{
   if(suiteRunner != 0)
   {
      delete suiteRunner;
   }

   if(fixtureRunner != 0)
   {
      TestFixtureRunnerFactory::
           destroyInstance(fixtureRunner);
   }

   if(resultManager != 0)
   {
      delete resultManager;
   }
}

///////////////////////////////////////////////////////
void
TestRunnerImpl::
loadListeners
   ( const StringList& searchingPaths
   , const StringList& listeners)
{
   resultManager->load(searchingPaths, listeners);
}

///////////////////////////////////////////////////////
void
TestRunnerImpl::createSuiteRunner(bool useSandbox, unsigned int maxConcurrent)
{
   fixtureRunner = TestFixtureRunnerFactory:: \
         createInstance(useSandbox, maxConcurrent);

   //TestSuiteLoader* loader = ModuleTestSuiteLoaderFactory().create();
   suiteRunner = new TestSuiteRunner(fixtureRunner, resultManager->getResultCollector());
}

///////////////////////////////////////////////////////
void TestRunnerImpl::runTestSuite
      (TestSuiteContext* suite)
{
   __TESTNGPP_TRY
   {
     suiteRunner->run(suite);
   }
   __TESTNGPP_CATCH(Error& e)
   {
      std::cerr << e.what() << std::endl;
      hasFailures = true;
   }
   __TESTNGPP_CATCH_ALL
   {
      std::cerr << TESTNGPP_INTERNAL_ERROR(5001) << std::endl;
      hasFailures = true;
   }
   __TESTNGPP_END_TRY
}

///////////////////////////////////////////////////////
void
TestRunnerImpl::runTests( const StringList& searchingPaths
                        , const StringList& suites
                        , const TestFilter* filter)
{
   resultManager->startTest();

#if 0
   StringList::Type::const_iterator i = suites.get().begin();
   for(; i != suites.get().end(); i++)
   {
      runTestSuite(searchingPaths, *i, filter);
   }
#endif

   resultManager->endTest();

   if(resultManager->hasFailure())
   {
      hasFailures = true;
   }
}

///////////////////////////////////////////////////////
TestRunner::TestRunner()
   : This(new TestRunnerImpl())
{
}

///////////////////////////////////////////////////////
TestRunner::~TestRunner()
{
   delete This;
}

///////////////////////////////////////////////////////
int
TestRunner::runTests( bool useSandbox
                    , unsigned int maxConcurrent
                    , const StringList& suitePaths
                    , const StringList& listenerNames
                    , const StringList& searchingPaths
                    , const StringList& fixtures)
{
   This->createSuiteRunner(useSandbox, maxConcurrent);

   This->loadListeners(searchingPaths, listenerNames);

   const TestFilter* filter = TestFilterFactory::getFilter(fixtures);

   This->runTests(searchingPaths, suitePaths, filter);

   TestFilterFactory::returnFilter(filter);

   return This->hasFailures ? -2 : 0;
}

///////////////////////////////////////////////////////

TESTNGPP_NS_END

