
#include <sstream>
#include <vector>

#include <errno.h>

#include <testngppst/utils/InternalError.h>

#include <testngppst/internal/AssertionFailure.h>
#include <testngppst/internal/Warning.h>
#include <testngppst/internal/Info.h>

#include <testngppst/runner/TestCaseSandboxResultReporter.h>
#include <testngppst/runner/TestCaseSandboxResultDecoder.h>

#include <testngppst/comm/WrittableChannel.h>
#include <testngppst/comm/ReadableChannel.h>

TESTNGPPST_NS_START

namespace
{
   struct InfoContainer
   {
      void flush( const TestCaseInfoReader* testcase
                , TestCaseResultCollector* collector)
      {
        unsigned int indexOfInfos = 0;
        unsigned int indexOfWarns = 0;
 
        for(unsigned int i=0; i<types.size(); i++)
        {
           if(types[i]) collector->addCaseInfo(testcase, infos[indexOfInfos++]);
           else      collector->addCaseWarning(testcase, warns[indexOfWarns++]);
        }

        types.clear();
        infos.clear();
        warns.clear();
      }

      void addWarning(const Warning& warn)
      {
         types.push_back(false);
         warns.push_back(warn);
      }

      void addInfo(const Info& info)
      {
         types.push_back(true);
         infos.push_back(info);
      }

      std::vector<bool> types;
      std::vector<Info> infos;
      std::vector<Warning> warns;
   };
}

struct TestCaseSandboxResultDecoderImpl
{
	void addCaseError(const std::string& msg);
	void addCaseFailure(const AssertionFailure& failure);
	void addCaseInfo(const Info& info);
	void addCaseWarning(const Warning& warning);

   AssertionFailure readAssertionFailure();
   Warning readWarning();
   Info readInfo();

   void handleInfo();
   void handleWarning();
   void handleAssertionFailure();
   void handleError();
   void handleInternalError();
   void handleStartCase();
   void handleEndCase();

   void flushRegularEvents();
   void flushErrorEvents();
   void flushFailureEvents();
   void flushInfoEvents();
   void flushEndEvent();

   bool decode();
   void flush(bool crashed);

   TestCaseSandboxResultDecoderImpl(ReadableChannel* ch
             , const TestCaseInfoReader* tc
             , TestCaseResultCollector* rc
             , bool report)
      : channel(ch), testcase(tc), collector(rc)
      , startReceived(false), endReceived(false)
      , errorReceived(false), failureReceived(false)
      , infoReceived(false)
      , crashInformed(false), reportSuccess(report)
   {}

   ~TestCaseSandboxResultDecoderImpl()
   {
      if(channel != 0)
      { delete channel; }
   }

   bool hasError() const
   { return errorReceived || failureReceived || crashInformed; }

   bool couldRecvEvents() const
   { return (startReceived && !endReceived && !crashInformed); }

   ReadableChannel* channel; // Y

   const TestCaseInfoReader* testcase; // X
   TestCaseResultCollector* collector; // X
  

   typedef std::vector<std::string> Errors;
   typedef std::vector<AssertionFailure> Failures;

   InfoContainer infos;
   
   Errors errors;
   Failures failures;
   bool startReceived;
   bool endReceived;
   bool errorReceived;
   bool failureReceived;
   bool infoReceived;
   bool crashInformed;
   bool reportSuccess;
};

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
addCaseError(const std::string& msg)
{
   if(!couldRecvEvents())
   {
      throw Error(TESTNGPPST_INTERNAL_ERROR(1001));
   }

   errors.push_back(msg);

   errorReceived = true;
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
addCaseFailure(const AssertionFailure& failure)
{
   if(!couldRecvEvents())
   {
      throw Error(TESTNGPPST_INTERNAL_ERROR(1002));
   }

   failures.push_back(failure);

   failureReceived = true;
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
addCaseInfo(const Info& info)
{
   if(!couldRecvEvents())
   {
      throw Error(TESTNGPPST_INTERNAL_ERROR(1006));
   }

   infos.addInfo(info);
  
   infoReceived = true;
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
addCaseWarning(const Warning& warning)
{
   if(!couldRecvEvents())
   {
      throw Error(TESTNGPPST_INTERNAL_ERROR(1007));
   }

   infos.addWarning(warning);
  
   infoReceived = true;
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
flushErrorEvents()
{
   Errors::iterator error = errors.begin();
   for(; error != errors.end(); error++)
   {
      collector->addCaseError(testcase, (*error));
   }

   errors.clear();
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
flushFailureEvents()
{
   Failures::iterator failure = failures.begin();
   for(; failure != failures.end(); failure++)
   {
      collector->addCaseFailure(testcase, (*failure));
   }

   failures.clear();
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
flushInfoEvents()
{
   infos.flush(testcase, collector);
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
flushEndEvent()
{
   if(endReceived)
   {
      collector->endTestCase(testcase);
   }
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
flushRegularEvents()
{
   if(!startReceived)
   {
      return;
   }

   if(!(reportSuccess || hasError()))
   { 
      return;
   }

   collector->startTestCase(testcase);

   flushInfoEvents();
   flushErrorEvents();
   flushFailureEvents();

   flushEndEvent();
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
flush(bool crashed)
{
   if(crashInformed)
   {
      return;
   }

   flushRegularEvents();

   if(crashed && !endReceived)
   {
      collector->addCaseCrash(testcase);
      collector->endTestCase(testcase);
      crashInformed = true;
   }
}

/////////////////////////////////////////////////////////////////////////
namespace
{
   const unsigned char startCmd   = 1;
   const unsigned char endCmd     = 2;
   const unsigned char errorCmd   = 3;
   const unsigned char failureCmd = 4;
   const unsigned char infoCmd    = 5;
   const unsigned char warningCmd = 6;
}

/////////////////////////////////////////////////////////////
AssertionFailure
TestCaseSandboxResultDecoderImpl::
readAssertionFailure()
{
   std::string file = channel->readString();
   int line = channel->readInt();
   std::string reason = channel->readString();
   return AssertionFailure(file, line, reason);
}

/////////////////////////////////////////////////////////////
Info
TestCaseSandboxResultDecoderImpl::
readInfo()
{
   std::string file = channel->readString();
   int line = channel->readInt();
   std::string reason = channel->readString();
   return Info(file, line, reason);
}

/////////////////////////////////////////////////////////////
Warning
TestCaseSandboxResultDecoderImpl::
readWarning()
{
   std::string file = channel->readString();
   int line = channel->readInt();
   std::string reason = channel->readString();
   return Warning(file, line, reason);
}

/////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
handleAssertionFailure()
{
   addCaseFailure(readAssertionFailure());
}

/////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
handleInfo()
{
   addCaseInfo(readInfo());
}

/////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
handleWarning()
{
   addCaseWarning(readWarning());
}

/////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
handleError()
{
   addCaseError(channel->readString());
}

/////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
handleInternalError()
{
   const char * err = TESTNGPPST_INTERNAL_ERROR(1003);
   addCaseError(err);
   throw Error(err);
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
handleStartCase()
{
   if( startReceived   ||
       endReceived     || 
       errorReceived   ||
       failureReceived || 
       crashInformed)
   {
      throw Error(TESTNGPPST_INTERNAL_ERROR(1004));
   }

   startReceived = true;
}

/////////////////////////////////////////////////////////////////////////
void
TestCaseSandboxResultDecoderImpl::
handleEndCase()
{
   if( endReceived   ||
      !startReceived ||
       crashInformed)
   {
      throw Error(TESTNGPPST_INTERNAL_ERROR(1005));
   }

   endReceived = true;
}
/////////////////////////////////////////////////////////////////////////
bool TestCaseSandboxResultDecoderImpl::decode()
{
   switch(channel->readByte())
   {
   case startCmd:
      handleStartCase(); break;
   case endCmd:
      handleEndCase();
      return true;
   case errorCmd:
      handleError(); break;
   case failureCmd:
      handleAssertionFailure(); break;
   case infoCmd:
      handleInfo(); break;
   case warningCmd:
      handleWarning(); break;
   default:
      handleInternalError();
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
TestCaseSandboxResultDecoder::
TestCaseSandboxResultDecoder
      ( ReadableChannel* channel
      , const TestCaseInfoReader* testcase
      , TestCaseResultCollector* collector
      , bool shouldReport)
      : This(new TestCaseSandboxResultDecoderImpl
          (channel, testcase, collector, shouldReport))
{
}

/////////////////////////////////////////////////////////////////////////
TestCaseSandboxResultDecoder::~TestCaseSandboxResultDecoder()
{
   delete This;
}

/////////////////////////////////////////////////////////////////////////
bool TestCaseSandboxResultDecoder::decode()
{
   return This->decode();
}

/////////////////////////////////////////////////////////////////////////
bool TestCaseSandboxResultDecoder::hasError() const
{
   return This->hasError();
}

/////////////////////////////////////////////////////////////////////////
void TestCaseSandboxResultDecoder::flush(bool crashed)
{
   This->flush(crashed);
}

/////////////////////////////////////////////////////////////////////////

TESTNGPPST_NS_END

