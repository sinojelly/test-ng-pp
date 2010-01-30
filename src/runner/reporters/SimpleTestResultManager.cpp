
#include <iostream>

#include <testngpp/Error.h>
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/runner/TestSuiteRunner.h>
#include <testngpp/runner/LTTestSuiteLoader.h>
#include <testngpp/runner/LTTestListenerLoader.h>
#include <testngpp/runner/SimpleTestResultDispatcher.h>
#include <testngpp/runner/SimpleTestResultReporter.h>
#include <testngpp/runner/SimpleTestCaseResultReporter.h>
#include <testngpp/runner/SimpleTestSuiteResultReporter.h>
#include <testngpp/runner/TestFixtureRunnerFactory.h>
#include <testngpp/runner/InternalError.h>
#include <testngpp/runner/TestRunner.h>
#include <testngpp/runner/TestFilter.h>
#include <testngpp/runner/TestFilterFactory.h>
#include <testngpp/runner/ModuleLoaderFactory.h>

TESTNGPP_NS_START

struct TestRunnerImpl
{
   typedef std::list<TestListenerLoader*> Listeners;

   Listeners listeners;

   SimpleTestResultReporter* reporter;
   SimpleTestSuiteResultReporter* suiteReporter;
   SimpleTestCaseResultReporter* caseReporter;
   SimpleTestResultDispatcher* dispatcher;

   TestRunnerImpl();
   ~TestRunnerImpl();

   
   void loadListener(TestRunnerContext* context, \
            const TestRunner::StringList& searchingPaths,
            const std::string& listenerName);

   void loadListeners(TestRunnerContext* context, \
            const TestRunner::StringList& searchingPaths,
            const TestRunner::StringList& listenerNames);

   void clearListeners();

   ModuleLoader* createModuleLoader();
};

///////////////////////////////////////////////////////
TestRunnerImpl::TestRunnerImpl()
   : caseReporter(new SimpleTestCaseResultReporter())
   , dispatcher(new SimpleTestResultDispatcher())
{
   suiteReporter = new SimpleTestSuiteResultReporter(caseReporter);
   reporter = new SimpleTestResultReporter(suiteReporter);
   dispatcher->registerTestCaseListener(caseReporter);
   dispatcher->registerTestSuiteListener(suiteReporter);
   dispatcher->registerListener(reporter);
}

///////////////////////////////////////////////////////
void TestRunnerImpl::clearListeners()
{
   Listeners::iterator i = listeners.begin();
   for(; i != listeners.end(); i++)
   {
      delete (*i);
   }

   listeners.clear();
}

///////////////////////////////////////////////////////
TestRunnerImpl::~TestRunnerImpl()
{
   delete dispatcher;
   delete caseReporter;
   delete reporter;
   
   clearListeners();
}

///////////////////////////////////////////////////////
ModuleLoader*
TestRunnerImpl::createModuleLoader()
{
   ModuleLoader* moduleLoader = ModuleLoaderFactory::create();
   if(moduleLoader == 0)
   {
      throw Error("cannot create module loader");
   }

   return moduleLoader;
}

///////////////////////////////////////////////////////
void
TestRunnerImpl::
loadListener( TestRunnerContext* context
            , const TestRunner::StringList& searchingPaths
            , const std::string& listenerName)
{
   __TESTNGPP_TRY
   {
      TestListenerLoader* loader = \
         new LTTestListenerLoader(createModuleLoader(), listenerName);
      loader->load(context, searchingPaths);
      listeners.push_back(loader);
   }
   __TESTNGPP_CATCH(Error& e)
   {
      std::cerr << "error occured while loading listener " 
                << listenerName 
                << " : " 
                << e.what() << std::endl;
   }
   __TESTNGPP_END_TRY
}

///////////////////////////////////////////////////////
void
TestRunnerImpl::
loadListeners( TestRunnerContext* context \
             , const TestRunner::StringList& searchingPaths
             , const TestRunner::StringList& listenerNames)
{
   TestRunner::StringList::const_iterator i = listenerNames.begin();
   for(; i != listenerNames.end(); i++)
   {
      loadListener(context, searchingPaths, *i);
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
TestResultReporter* 
TestRunner::getTestResultReporter() const
{
   return This->reporter;
}

///////////////////////////////////////////////////////
TestSuiteResultReporter* 
TestRunner::getTestSuiteResultReporter() const
{
   return This->suiteReporter;
}

///////////////////////////////////////////////////////
TestCaseResultReporter* 
TestRunner::getTestCaseResultReporter() const
{
   return This->caseReporter;
}

///////////////////////////////////////////////////////
void
TestRunner::registerTestListener(TestListener* listener)
{
   This->dispatcher->registerListener(listener);
}

///////////////////////////////////////////////////////

TESTNGPP_NS_END

