
// for select
#include <sys/select.h>
#include <errno.h>
#include <list>
#include <algorithm>
#include <string.h>

#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/runner/TestHierarchySandboxRunner.h>
#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/runner/TestCaseRunner.h>
#include <testngpp/runner/TestCaseSandboxResultReporter.h>
#include <testngpp/runner/TestCaseSandboxResultDecoder.h>
#include <testngpp/runner/TestFixtureResultCollector.h>
#include <testngpp/runner/TestCaseSandbox.h>
#include <testngpp/runner/EnvironmentCleaner.h>
#include <testngpp/runner/TestCaseFilter.h>
#include <testngpp/runner/TestCaseContainer.h>


TESTNGPP_NS_START

///////////////////////////////////////////////////////
struct TestHierarchySandboxRunnerImpl 
   : public EnvironmentCleaner
{
   typedef std::list<TestCaseSandbox*> List;

	TestHierarchySandboxRunnerImpl(unsigned int maxCurrentProcess, TestCaseRunner* runner)
      : maxProcess(maxCurrentProcess), caseRunner(runner), index(0)
   {}

	~TestHierarchySandboxRunnerImpl()
   {
      cleanUp();
   }

	void run(TestCaseHierarchy* hierarchy
      , TestFixtureResultCollector* resultCollector
      , const TestCaseFilter* filter);

   void setupListeners();
   void createSandbox( TestCaseHierarchy* hierarchy, unsigned int i
             , TestFixtureResultCollector* resultCollector);


   void createSandboxes(TestCaseHierarchy* hierarchy
             , TestFixtureResultCollector* resultCollector
             , const TestCaseFilter* filter);

   void cleanUpDeadSandboxes();

   void process(TestCaseHierarchy* hierarchy);
   void handleEvent(int nfds, TestCaseHierarchy* hierarchy);

   void cleanUp();

   const unsigned int maxProcess;
   TestCaseRunner* caseRunner;
   unsigned int    index;
   fd_set          fds;
   int             maxfd;
   List sandboxes;
};

////////////////////////////////////////////////////
namespace 
{
   static void removeSandbox(TestCaseSandbox* sandbox)
   {
      delete sandbox;
   }
}

////////////////////////////////////////////////////
void TestHierarchySandboxRunnerImpl::cleanUp()
{
   std::for_each(sandboxes.begin(), sandboxes.end(), removeSandbox);
   sandboxes.clear();
}

////////////////////////////////////////////////////
namespace
{
    static bool isDead(TestCaseSandbox* sandbox)
    {
       return sandbox->isDead();  
    }
}

////////////////////////////////////////////////////
void TestHierarchySandboxRunnerImpl::cleanUpDeadSandboxes()
{
   while(1)
   {
      List::iterator i = std::find_if(sandboxes.begin(), sandboxes.end(), isDead);
      if(i == sandboxes.end())
      {
         break;
      }

      (*i)->cleanup();
      delete *i;
      sandboxes.erase(i);
   }
}

////////////////////////////////////////////////////
void TestHierarchySandboxRunnerImpl::
createSandbox( TestCaseHierarchy* hierarchy, unsigned int i
             , TestFixtureResultCollector* resultCollector)
{
#if 0
   TestCaseSandbox* sandbox = \
         TestCaseSandbox::createInstance
               ( this, hierarchy->getTestCase(i)
               , caseRunner, resultCollector);

   sandboxes.push_back(sandbox);
#endif
}

////////////////////////////////////////////////////
void TestHierarchySandboxRunnerImpl::
createSandboxes
      ( TestCaseHierarchy* hierarchy
      , TestFixtureResultCollector* resultCollector
      , const TestCaseFilter* filter)
{
#if 0
   unsigned int numberOfTestCases = hierarchy->getNumberOfTestCases();
   unsigned int i = index;
   for(; i < numberOfTestCases && sandboxes.size() < maxProcess; i++)
   {
      if(filter->isCaseMatch((const TestCaseInfoReader*)hierarchy->getTestCase(i)))
      {
         createSandbox(hierarchy, i, resultCollector);
      }
   }

   index = i;
#endif
}


/////////////////////////////////////////////////////
void TestHierarchySandboxRunnerImpl::setupListeners()
{
   FD_ZERO(&fds);
   maxfd = 0;

   List::iterator i = sandboxes.begin();
   for(; i != sandboxes.end(); i++)
   {
      FD_SET((*i)->getChannelId(), &fds);
      maxfd = maxfd < (*i)->getChannelId() ? (*i)->getChannelId() : maxfd;
   }
}

///////////////////////////////////////////////////////
namespace
{
   static void handleSandboxEvent(TestCaseSandbox* sandbox)
   {
      sandbox->handle();
   }
}
///////////////////////////////////////////////////////
void TestHierarchySandboxRunnerImpl::
handleEvent(int nfds, TestCaseHierarchy* hierarchy)
{
   List::iterator i = sandboxes.begin();
   for(; i != sandboxes.end(); i++)
   {
      if(FD_ISSET((*i)->getChannelId(), &fds))
      {
         handleSandboxEvent(*i);
         if(--nfds <= 0)
         {
            break;
         }
      }
   }

   cleanUpDeadSandboxes();
}

///////////////////////////////////////////////////////
void TestHierarchySandboxRunnerImpl::
process(TestCaseHierarchy* hierarchy)
{
   setupListeners();

   int nfds = 0;
   do{
      nfds = ::select(maxfd + 1, &fds, 0, 0, 0);
   }while(nfds == 0 || (nfds < 0 && errno == EAGAIN));

   if(nfds < 0)
   {
      throw Error(strerror(errno));
   }

   handleEvent(nfds, hierarchy);
}

///////////////////////////////////////////////////////
void
TestHierarchySandboxRunnerImpl::
run( TestCaseHierarchy* hierarchy
   , TestFixtureResultCollector* resultCollector
   , const TestCaseFilter* filter)
{
   index = 0;

   while(1)
   {
      createSandboxes(hierarchy, resultCollector, filter);
      if(sandboxes.size() == 0)
      {
         break;
      }

      process(hierarchy);
   }
}

///////////////////////////////////////////////////////
TestHierarchySandboxRunner::TestHierarchySandboxRunner(
     unsigned int maxCurrentProcess
   , TestCaseRunner* caseRunner)
   : This(new TestHierarchySandboxRunnerImpl(maxCurrentProcess, caseRunner))
{
}

///////////////////////////////////////////////////////
TestHierarchySandboxRunner::~TestHierarchySandboxRunner()
{
   delete This;
}

///////////////////////////////////////////////////////
void
TestHierarchySandboxRunner::run(TestCaseHierarchy* hierarchy
      , TestFixtureResultCollector* resultCollector
      , const TestCaseFilter* filter)
{
   This->run(hierarchy, resultCollector, filter);
}

///////////////////////////////////////////////////////

TESTNGPP_NS_END

