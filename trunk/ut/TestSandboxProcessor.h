
#include <unistd.h>

#include <cxxtest/TestSuite.h> 
#include <testngpp/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/internal/TestCase.h>
#include <testngpp/runner/SandboxProcessor.h>
#include <testngpp/runner/SandboxHandler.h>
#include <testngpp/internal/TestCaseInfoReader.h>
#include <testngpp/runner/EnvironmentCleaner.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestSandboxProcessor: public CxxTest::TestSuite
{
private:
   TESTNGPP_RCP checkpoint;

public:
   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();
   }
   void tearDown()
   {
      TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
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
