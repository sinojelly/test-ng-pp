
#include <cxxtest/TestSuite.h> 
#include <testcpp/ResourceCheckPoint.h>

#include <mockcpp.hpp>

#include <Sandbox.h>
#include <EnvironmentCleaner.h>
#include <SandboxHandler.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestSandbox: public CxxTest::TestSuite
{

   MockObject<EnvironmentCleaner> cleaner;
   MockObject<SandboxHandler> handler;

private:
   TESTCPP_RCP checkpoint;

public:

   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();

      cleaner.METHOD(EnvironmentCleaner::cleanUp).stubs();
      handler.METHOD(SandboxHandler::handle).stubs();
   }

   void tearDown()
   {
      cleaner.reset();
      handler.reset();

      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleCreateInstance()
   {
      Sandbox* sandbox = Sandbox::createInstance(cleaner, handler);
      TS_ASSERT(sandbox != 0);
      TS_ASSERT(sandbox->getChannelId() > 0);
      TS_ASSERT(sandbox->getSandboxId() > 0);
      sandbox->cleanup();
      delete sandbox;
   }

   void testShouldBeAbleToLetSandboxDie()
   {
      Sandbox* sandbox = Sandbox::createInstance(cleaner, handler);
      TS_ASSERT(sandbox != 0);

      TS_ASSERT(!sandbox->isDead());

      sandbox->die();

      TS_ASSERT(sandbox->isDead());

      sandbox->cleanup();
      delete sandbox;
   }
};
