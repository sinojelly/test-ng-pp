
#include <testngpp/runner/TestFixtureRunner.h>
#include <testngpp/runner/SimpleTestHierarchyRunner.h>
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestFixtureRunnerFactory.h>

#if !defined(TESTNGPP_DISABLE_SANDBOX) || !TESTNGPP_DISABLE_SANDBOX
#if defined(_MSC_VER)
#include <testngpp/win32/Win32TestHierarchySandboxRunner.h>
#else
#include <testngpp/runner/TestHierarchySandboxRunner.h>
#endif
#endif

TESTNGPP_NS_START

////////////////////////////////////////////////////////
namespace
{
   unsigned int ref = 0;

   TestCaseRunner* caseRunner = 0;

   TestCaseRunner* createTestCaseRunner()
   {
      if(caseRunner == 0)
      {
         caseRunner = new SimpleTestCaseRunner();
         ref = 1;
      }
      else
      {
         ref++;
      }

      return caseRunner;
   }

   void freeTestCaseRunner()
   {
      if(ref == 0)
      {
         return;
      }

      if(--ref == 0)
      {
         delete caseRunner;
         caseRunner = 0;
      }
   }
   
#if !defined(TESTNGPP_DISABLE_SANDBOX) || !TESTNGPP_DISABLE_SANDBOX
   TestFixtureRunner*
   createSandboxInstance(unsigned int maxConcurrent)
   {
      if(maxConcurrent == 0)
      {
         maxConcurrent = 1;
      }

      return new TestFixtureRunner( 
#if defined(_MSC_VER)
		          new Win32TestHierarchySandboxRunner(
#else
		          new TestHierarchySandboxRunner( 
#endif
                     maxConcurrent, createTestCaseRunner()));
   }
#endif

   TestFixtureRunner*
   createSimpleInstance()
   {
      return new TestFixtureRunner( \
                  new SimpleTestHierarchyRunner( \
                       createTestCaseRunner()));
   }
}


////////////////////////////////////////////////////////
TestFixtureRunner*
TestFixtureRunnerFactory::
createInstance(bool useSandbox, unsigned int maxConcurrent)
{
#if defined(TESTNGPP_DISABLE_SANDBOX) && TESTNGPP_DISABLE_SANDBOX
   return createSimpleInstance();
#else
   return useSandbox? createSandboxInstance(maxConcurrent) :
      createSimpleInstance();
#endif
}

////////////////////////////////////////////////////////
void 
TestFixtureRunnerFactory::
destroyInstance(TestFixtureRunner* instance)
{
   if(instance == 0)
   {
      return;
   }

   delete instance;

   freeTestCaseRunner();
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
