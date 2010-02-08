
#include <list>
#include <algorithm>

#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/win32/Win32TestHierarchySandboxRunner.h>
#include <testngpp/runner/TestCaseRunner.h>
#include <testngpp/runner/TestCaseSandboxResultDecoder.h>
#include <testngpp/runner/TestFixtureResultCollector.h>
#include <testngpp/win32/Win32TestCaseSandbox.h>
#include <testngpp/runner/TestHierarchyHandler.h>


TESTNGPP_NS_START

///////////////////////////////////////////////////////
struct Win32TestHierarchySandboxRunnerImpl 
{
   typedef std::list<Win32TestCaseSandbox*> List;

	Win32TestHierarchySandboxRunnerImpl
      ( unsigned int maxCurrentProcess
      , TestCaseRunner* runner)
      : maxProcess(maxCurrentProcess)
      , caseRunner(runner)
      , index(0)
   {}

	~Win32TestHierarchySandboxRunnerImpl()
   {
      cleanUp();
   }

	void run(TestHierarchyHandler* hierarchy
      , TestFixtureResultCollector* resultCollector);

   void setupListeners();
   void createSandbox( TestHierarchyHandler* handler, unsigned int i
             , TestFixtureResultCollector* resultCollector);


   void createSandboxes(TestHierarchyHandler* handler 
             , TestFixtureResultCollector* resultCollector);

   void cleanUpDeadSandboxes(TestHierarchyHandler*);

   void process(TestHierarchyHandler* hierarchy);
   void handleEvent(int nfds, TestHierarchyHandler* hierarchy);

   void cleanUp();

   const unsigned int maxProcess;
   TestCaseRunner* caseRunner;
   unsigned int    index;
   List sandboxes;
};

////////////////////////////////////////////////////
namespace 
{
   static void removeSandbox(Win32TestCaseSandbox* sandbox)
   {
      delete sandbox;
   }
}

////////////////////////////////////////////////////
void Win32TestHierarchySandboxRunnerImpl::cleanUp()
{
   std::for_each(sandboxes.begin(), sandboxes.end(), removeSandbox);
   sandboxes.clear();
}

////////////////////////////////////////////////////
namespace
{
    static bool isDead(Win32TestCaseSandbox* sandbox)
    {
       return sandbox->isDead();  
    }
}

////////////////////////////////////////////////////
void Win32TestHierarchySandboxRunnerImpl::
cleanUpDeadSandboxes(TestHierarchyHandler* handler)
{
   while(1)
   {
      List::iterator i = std::find_if(sandboxes.begin(), sandboxes.end(), isDead);
      if(i == sandboxes.end())
      {
         break;
      }

      handler->testDone((*i)->getTestCase(), (*i)->hasSucceeded());

      (*i)->cleanup();
      delete *i;
      sandboxes.erase(i);
   }
}

////////////////////////////////////////////////////
void Win32TestHierarchySandboxRunnerImpl::
createSandbox
      ( TestHierarchyHandler* handler
	  , unsigned int i
      , TestFixtureResultCollector* resultCollector)
{
   TestHierarchyHandler::ValueType testcase = handler->getTestCase(i);
   Win32TestCaseSandbox* sandbox = \
         Win32TestCaseSandbox::createInstance
               ( testcase.first
               , caseRunner
               , resultCollector
               , testcase.second);

   sandboxes.push_back(sandbox);
}

////////////////////////////////////////////////////
void Win32TestHierarchySandboxRunnerImpl::
createSandboxes
      ( TestHierarchyHandler* handler
      , TestFixtureResultCollector* resultCollector)
{
   unsigned int numberOfTestCases = handler->numberOfTestCasesInSched();
   unsigned int i = index;
   for(; i < numberOfTestCases && sandboxes.size() < maxProcess; i++)
   {
      createSandbox(handler, i, resultCollector);
   }

   index = i;
}


/////////////////////////////////////////////////////
void Win32TestHierarchySandboxRunnerImpl::setupListeners()
{
#if 0
   FD_ZERO(&fds);
   maxfd = 0;

   List::iterator i = sandboxes.begin();
   for(; i != sandboxes.end(); i++)
   {
      FD_SET((*i)->getChannelId(), &fds);
      maxfd = maxfd < (*i)->getChannelId() ? (*i)->getChannelId() : maxfd;
   }
#endif   
}

///////////////////////////////////////////////////////
namespace
{
   static void handleSandboxEvent(Win32TestCaseSandbox* sandbox)
   {
      sandbox->handle();
   }
}
///////////////////////////////////////////////////////
void Win32TestHierarchySandboxRunnerImpl::
handleEvent(int nfds, TestHierarchyHandler* handler)
{
#if 0
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
#endif
   cleanUpDeadSandboxes(handler);
}

///////////////////////////////////////////////////////
void Win32TestHierarchySandboxRunnerImpl::
process(TestHierarchyHandler* handler)
{
   setupListeners();

#if 0   
   int nfds = 0;
   do{
      nfds = ::select(maxfd + 1, &fds, 0, 0, 0);
   }while(nfds == 0 || (nfds < 0 && errno == EAGAIN));

   if(nfds < 0)
   {
      throw Error(strerror(errno));
   }

   handleEvent(nfds, handler);
#endif   
}

///////////////////////////////////////////////////////
void
Win32TestHierarchySandboxRunnerImpl::
run( TestHierarchyHandler* handler
   , TestFixtureResultCollector* resultCollector)
{
   index = 0;

   while(1)
   {
      createSandboxes(handler, resultCollector);
      if(sandboxes.size() == 0)
      {
         break;
      }

      process(handler);
   }
}

///////////////////////////////////////////////////////
Win32TestHierarchySandboxRunner::
Win32TestHierarchySandboxRunner(
     unsigned int maxCurrentProcess
   , TestCaseRunner* caseRunner)
   : This(new Win32TestHierarchySandboxRunnerImpl(maxCurrentProcess, caseRunner))
{
}

///////////////////////////////////////////////////////
Win32TestHierarchySandboxRunner::
~Win32TestHierarchySandboxRunner()
{
   delete This;
}

///////////////////////////////////////////////////////
void
Win32TestHierarchySandboxRunner::
run(TestHierarchyHandler* handler
      , TestFixtureResultCollector* resultCollector)
{
   This->run(handler, resultCollector);
}

///////////////////////////////////////////////////////

TESTNGPP_NS_END

