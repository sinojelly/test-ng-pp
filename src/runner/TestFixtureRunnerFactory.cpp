
#include <testngpp/runner/TestFixtureRunnerWrapper.h>
#include <testngpp/runner/TestFixtureSandboxRunner.h>
#include <testngpp/runner/SimpleTestFixtureRunner.h>
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestFixtureRunnerFactory.h>

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
   
   TestFixtureRunner*
   createSandboxInstance(unsigned int maxConcurrent)
   {
      if(maxConcurrent == 0)
      {
         maxConcurrent = 1;
      }

      return new TestFixtureRunnerWrapper( \
                  new TestFixtureSandboxRunner( \
                     maxConcurrent, createTestCaseRunner()));
   }

   TestFixtureRunner*
   createSimpleInstance()
   {
      return new TestFixtureRunnerWrapper( \
                  new SimpleTestFixtureRunner( \
                       createTestCaseRunner()));
   }
}


////////////////////////////////////////////////////////
TestFixtureRunner*
TestFixtureRunnerFactory::
createInstance(bool useSandbox, unsigned int maxConcurrent)
{
   return useSandbox? createSandboxInstance(maxConcurrent) :
      createSimpleInstance();
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
