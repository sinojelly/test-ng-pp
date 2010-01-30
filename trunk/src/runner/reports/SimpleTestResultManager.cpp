
#include <iostream>

#include <testngpp/Error.h>
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/runner/SimpleTestResultDispatcher.h>
#include <testngpp/runner/SimpleTestResultReporter.h>
#include <testngpp/runner/SimpleTestCaseResultReporter.h>
#include <testngpp/runner/SimpleTestSuiteResultReporter.h>

#include <testngpp/runner/TestListenerLoader.h>
#include <testngpp/runner/TestListenerLoaderFactory.h>

#include <testngpp/runner/InternalError.h>

#include <testngpp/runner/SimpleTestResultManager.h>

TESTNGPP_NS_START

struct SimpleTestResultManagerImpl
{
   typedef std::list<TestListenerLoader*> Loaders;

   Loaders loaders;

   TestListenerLoaderFactory* loaderFactory;

   SimpleTestResultReporter* reporter;
   SimpleTestSuiteResultReporter* suiteReporter;
   SimpleTestCaseResultReporter* caseReporter;

   SimpleTestResultDispatcher* dispatcher;

   SimpleTestResultManagerImpl(TestListenerLoaderFactory* factory);
   ~SimpleTestResultManagerImpl();

   void loadListener \
            ( const StringList& searchingPaths
            , const std::string& listenerName);

   void loadListeners \
            ( const StringList& searchingPaths \
            , const StringList& listenerNames);

   void clearListeners();
};

///////////////////////////////////////////////////////
SimpleTestResultManagerImpl::
SimpleTestResultManagerImpl(TestListenerLoaderFactory* factory)
   : loaderFactory(factory)
{
   dispatcher = new SimpleTestResultDispatcher();

   caseReporter = new SimpleTestCaseResultReporter();
   suiteReporter = new SimpleTestSuiteResultReporter(caseReporter);
   reporter = new SimpleTestResultReporter(suiteReporter);

   dispatcher->registerTestCaseListener(caseReporter);
   dispatcher->registerTestSuiteListener(suiteReporter);
   dispatcher->registerListener(reporter);
}

///////////////////////////////////////////////////////
void SimpleTestResultManagerImpl::clearListeners()
{
   Loaders::iterator i = loaders.begin();
   for(; i != loaders.end(); i++)
   {
      delete (*i);
   }

   loaders.clear();
}

///////////////////////////////////////////////////////
SimpleTestResultManagerImpl::
~SimpleTestResultManagerImpl()
{
   delete suiteReporter;
   delete caseReporter;
   delete reporter;
   
   delete dispatcher;

   clearListeners();
}

///////////////////////////////////////////////////////
void
SimpleTestResultManagerImpl::
loadListener( const StringList& searchingPaths
            , const std::string& cl)
{
   __TESTNGPP_TRY
   {
      TestListenerLoader* loader = loaderFactory->create();
      TestListener* listener = \
         loader->load( searchingPaths, cl \
                     , reporter \
                     , suiteReporter \
                     , caseReporter);
      dispatcher->registerListener(listener);
      loaders.push_back(loader);
   }
   __TESTNGPP_CATCH(Error& e)
   {
      std::cerr << "error occured while loading listener " 
                << cl
                << " : " 
                << e.what() << std::endl;
   }
   __TESTNGPP_END_TRY
}

///////////////////////////////////////////////////////
void
SimpleTestResultManager::
load( const StringList& searchingPaths
    , const StringList& listenerNames)
{
   StringList::Type::const_iterator i = listenerNames.get().begin();
   for(; i != listenerNames.get().end(); i++)
   {
      This->loadListener(searchingPaths, *i);
   }
}

///////////////////////////////////////////////////////
SimpleTestResultManager::
SimpleTestResultManager(TestListenerLoaderFactory* loaderFactory)
   : This(new SimpleTestResultManagerImpl(loaderFactory))
{
}

///////////////////////////////////////////////////////
SimpleTestResultManager::
~SimpleTestResultManager()
{
   delete This;
}

///////////////////////////////////////////////////////

TESTNGPP_NS_END

