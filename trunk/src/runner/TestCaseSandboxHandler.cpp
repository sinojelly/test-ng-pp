
#include <testngpp/runner/TestCaseSandboxHandler.h>
#include <testngpp/runner/TestCaseResultCollector.h>
#include <testngpp/runner/TestCaseSandboxResultReporter.h>
#include <testngpp/runner/TestCaseRunner.h>
#include <testngpp/internal/TestCase.h>
#include <testngpp/comm/PipeWrittableChannel.h>

TESTNGPP_NS_START

//////////////////////////////////////////////////////
struct TestCaseSandboxHandlerImpl
{
   TestCaseSandboxHandlerImpl(TestCase* tc,
          TestFixtureDesc* tfd,
          TestCaseRunner* tcRunner)
      : testcase(tc), fixture(tfd), runner(tcRunner)
   {}

	void handle(ChannelId channelId);

   TestCase* testcase;
   TestFixtureDesc* fixture;
   TestCaseRunner* runner;
};


//////////////////////////////////////////////////////
void
TestCaseSandboxHandlerImpl::handle(ChannelId channelId)
{
   TestCaseResultCollector* reporter = \
         new TestCaseSandboxResultReporter(new PipeWrittableChannel(channelId));
   
   try {
      runner->run(fixture, testcase, reporter);
   }
   catch(...)
   {
      delete reporter;
      throw;
   }

   delete reporter;
}

//////////////////////////////////////////////////////
TestCaseSandboxHandler::TestCaseSandboxHandler(TestCase* testcase,
          TestFixtureDesc* fixture,
          TestCaseRunner* runner)
   : This(new TestCaseSandboxHandlerImpl(testcase, fixture, runner))
{
}

//////////////////////////////////////////////////////
TestCaseSandboxHandler::~TestCaseSandboxHandler()
{
   delete This;
}

//////////////////////////////////////////////////////
void
TestCaseSandboxHandler::handle(ChannelId channelId)
{
   This->handle(channelId);
}

//////////////////////////////////////////////////////

TESTNGPP_NS_END

