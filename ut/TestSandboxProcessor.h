
#include <cxxtest/TestSuite.h> 
#include <testcpp/ResourceCheckPoint.h>

#include <mockcpp.hpp>

#include <unistd.h>

#include <TestCase.h>
#include <SandboxProcessor.h>
#include <SandboxHandler.h>
#include <TestCaseInfoReader.h>
#include <EnvironmentCleaner.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestSandboxProcessor: public CxxTest::TestSuite
{
private:
   TESTCPP_RCP checkpoint;

public:
   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();
   }
   void tearDown()
   {
      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldCleanupEnvironmentAndDoHandling()
   {
      int fds[2];

      int result = ::pipe(fds);
      TS_ASSERT(result >= 0);

      MockObject<EnvironmentCleaner> cleaner;
      MockObject<SandboxHandler> handler;

      cleaner.METHOD(EnvironmentCleaner::cleanUp).expects(once());
      handler.METHOD(SandboxHandler::handle).expects(once());

      SandboxProcessor::process(fds[0], fds[1], cleaner, handler);
   }
};
