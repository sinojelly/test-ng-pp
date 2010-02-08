
#include <testngpp/Error.h>
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/comm/Win32PipeReadableChannel.h>

#include <testngpp/internal/TestCase.h>

#include <testngpp/runner/EOFError.h>
#include <testngpp/runner/TestCaseRunner.h>
#include <testngpp/runner/TestCaseSandboxResultDecoder.h>
#include <testngpp/runner/TestCaseResultCollector.h>

#include <testngpp/Win32/Win32Sandbox.h>
#include <testngpp/win32/Win32TestCaseSandbox.h>


TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct Win32TestCaseSandboxImpl
{
   Win32TestCaseSandboxImpl()
     : testcase(0), sandbox(0), decoder(0), finished(false)
   {}

   HANDLE getSandboxId() const
   { return sandbox->getSandboxId(); }
 
   HANDLE getChannelId() const
   {
     return sandbox->getChannelId();
   }

   HANDLE getEventId() const
   {
      return sandbox->getSandboxId();
   }
   
   void handle() TESTNGPP_THROW(EOFError, Error);

   ~Win32TestCaseSandboxImpl()
   {
      if(sandbox != 0)
         delete sandbox;

      if(decoder != 0)
         delete decoder;
   }

   bool hasSucceeded() const
   { return !decoder->hasError(); }

   const TestCase* testcase; // X
   Win32Sandbox* sandbox; // Y
   TestCaseSandboxResultDecoder* decoder; // Y
   bool finished;
};

////////////////////////////////////////////////////////
void Win32TestCaseSandboxImpl::
handle() TESTNGPP_THROW(EOFError, Error)
{
   if(sandbox->isDead())
   {
      return;
   }

   __TESTNGPP_TRY
   {
	   bool result = decoder->decode();
      if(result && !finished)
      {
         finished = true;
         decoder->flush(false);
      }
   }
   __TESTNGPP_CATCH(EOFError&)
   {
      if(!finished)
      {
         decoder->flush(true);
      }

      sandbox->die();

      finished = true;
   }
   __TESTNGPP_END_TRY
}

////////////////////////////////////////////////////////
Win32TestCaseSandbox::Win32TestCaseSandbox()
   : This(new Win32TestCaseSandboxImpl())
{}

////////////////////////////////////////////////////////
Win32TestCaseSandbox::
~Win32TestCaseSandbox()
{
	delete This;
}

////////////////////////////////////////////////////////
HANDLE 
Win32TestCaseSandbox::
getChannelId() const
{
   return This->getChannelId();
}

////////////////////////////////////////////////////////
bool 
Win32TestCaseSandbox::
hasSucceeded() const
{
   return This->hasSucceeded();
}

const TestCase*
Win32TestCaseSandbox::
getTestCase() const
{
   return This->testcase;
}

////////////////////////////////////////////////////////
void
Win32TestCaseSandbox::
handle() TESTNGPP_THROW(EOFError, Error)
{
   This->handle();
}

////////////////////////////////////////////////////////
bool
Win32TestCaseSandbox::
isDead() const
{
   return This->sandbox->isDead();
}
////////////////////////////////////////////////////////
void
Win32TestCaseSandbox::
cleanup()
{
   This->sandbox->cleanup();
}
////////////////////////////////////////////////////////
Win32TestCaseSandbox*
Win32TestCaseSandbox::
createInstance
       ( const TestCase* testcase
       , TestCaseRunner* runner
       , TestCaseResultCollector* collector
       , bool shouldReport)
{
   Win32Sandbox* sandbox = Win32Sandbox::createInstance(testcase);
   
   Win32TestCaseSandbox* tcSandbox = new Win32TestCaseSandbox();

   tcSandbox->This->decoder = \
         new TestCaseSandboxResultDecoder
            ( new Win32PipeReadableChannel(sandbox->getChannelId())
            , testcase
            , collector
            , shouldReport);

   tcSandbox->This->testcase = testcase;
   tcSandbox->This->sandbox = sandbox;

   return tcSandbox;
}

TESTNGPP_NS_END

