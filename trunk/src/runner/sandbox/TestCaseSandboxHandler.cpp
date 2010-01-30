
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/internal/TestCase.h>

#include <testngpp/comm/PipeWrittableChannel.h>

#include <testngpp/runner/TestCaseSandboxHandler.h>
#include <testngpp/runner/TestCaseResultCollector.h>
#include <testngpp/runner/TestCaseSandboxResultReporter.h>
#include <testngpp/runner/TestCaseRunner.h>


TESTNGPP_NS_START

//////////////////////////////////////////////////////
struct TestCaseSandboxHandlerImpl
{
   TestCaseSandboxHandlerImpl(TestCase* tc,
          TestCaseRunner* tcRunner)
      : testcase(tc), runner(tcRunner)
   {}

	void handle(ChannelId channelId);

   TestCase* testcase;
   TestCaseRunner* runner;
};

//////////////////////////////////////////////////////
void
TestCaseSandboxHandlerImpl::handle(ChannelId channelId)
{
   TestCaseResultCollector* reporter = \
         new TestCaseSandboxResultReporter( \
            new PipeWrittableChannel(channelId));
   
   __TESTNGPP_DO
   {
      runner->run(testcase, reporter);
   }
   __TESTNGPP_CLEANUP
   {
      delete reporter;
   }
   __TESTNGPP_DONE
}

//////////////////////////////////////////////////////
TestCaseSandboxHandler::TestCaseSandboxHandler(TestCase* testcase,
          TestCaseRunner* runner)
   : This(new TestCaseSandboxHandlerImpl(testcase, runner))
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

