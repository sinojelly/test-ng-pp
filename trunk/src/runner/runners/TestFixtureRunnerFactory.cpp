
#include <testngpp/runner/TestFixtureRunner.h>
#include <testngpp/runner/SimpleTestHierarchyRunner.h>
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestFixtureRunnerFactory.h>

#if !defined(TESTNGPP_DISABLE_SANDBOX) || !TESTNGPP_DISABLE_SANDBOX
#include <testngpp/runner/TestHierarchySandboxRunner.h>
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

      return new TestFixtureRunner( \
                  new TestHierarchySandboxRunner( \
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
