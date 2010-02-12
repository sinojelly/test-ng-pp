
#include <iostream>
#include <string>
#include <list>

#include <testngpp/Error.h>
#include <testngpp/AssertionFailure.h>

#include <testngpp/utils/OptionList.h>
#include <testngpp/utils/StringList.h>
#include <testngpp/utils/ConsoleState.h>

#include <testngpp/internal/TestCaseInfoReader.h>
#include <testngpp/internal/TestFixtureInfoReader.h>
#include <testngpp/internal/TestSuiteInfoReader.h>

#include <testngpp/runner/InternalError.h>
#include <testngpp/runner/TestListener.h>

#include <testngpp/runner/TaggableObjFilter.h>

#include <testngpp/runner/TestResultReporter.h>
#include <testngpp/runner/TestSuiteResultReporter.h>
#include <testngpp/runner/TestCaseResultReporter.h>

TESTNGPP_NS_START

namespace
{

   const unsigned int maxLenOfLine = 50;

   std::string
   formatTitle
      ( const std::string title
      , char pad
      , unsigned int maxLen)
   {
      unsigned int numberOfPadding = maxLen - title.size();
      if(title.size() > maxLen)
      {
         numberOfPadding = 0;
      }

      std::string leftPadding("");
      for(unsigned int i=0; i<numberOfPadding/2; i++)
      {
         leftPadding += pad;
      }

      std::string rightPadding("");
      for(unsigned int i=0; i<numberOfPadding/2+numberOfPadding%2; i++)
      {
         rightPadding = pad + rightPadding;
      }

      return leftPadding + title + rightPadding;
   }

   std::string
   getTitle(const std::string title)
   {
      return '[' + formatTitle(title, ' ', 10) + "] ";
   }   

   std::string
   getTitle(unsigned int code)
   {
      switch(code)
      {
      case TestCaseResultReporter::TR_SUCCESS:
         return getTitle("OK");
      case TestCaseResultReporter::TR_ERROR:
         return getTitle("ERROR");
      case TestCaseResultReporter::TR_CRASHED:
         return getTitle("CRASHED");
      case TestCaseResultReporter::TR_SKIPPED:
         return getTitle("SKIPPED");
      case TestCaseResultReporter::TR_FAILED:
         return getTitle("FAILED");
      }

      return getTitle("UNKNOWN");
   }

   struct TestCaseResult
   {
      TestCaseResult
         (const TestCaseInfoReader* testcase
         , unsigned int resultCode)
         : name(testcase->getName())
         , fixture(testcase->getNameOfFixture())
         , suite(testcase->getNameOfSuite())
         , result(resultCode)
      {}

      std::string toString() const
      {
          return getTitle(result) + suite + "::" + fixture + "::" + name;
      }
      
      std::string name;
      std::string fixture;
      std::string suite;
      unsigned int result;
   };
}
///////////////////////////////////////////////////////////
struct StdoutVerboseListener : public TestListener
{
   StdoutVerboseListener
      ( bool 
      , bool 
      , bool
      , bool
      , bool
      , TestResultReporter*
      , TestSuiteResultReporter*
      , TestCaseResultReporter* );
   
   void addCaseCrash(const TestCaseInfoReader*);
   void addCaseSkipped(const TestCaseInfoReader*);
   void addCaseError(const TestCaseInfoReader*, const std::string&);
   void addCaseFailure(const TestCaseInfoReader*, const AssertionFailure&);
   
   void startTestCase(const TestCaseInfoReader*);
   void endTestCase(const TestCaseInfoReader*);
   
   void startTestFixture(TestFixtureInfoReader*);
   void endTestFixture(TestFixtureInfoReader*);
   void addFixtureError(TestFixtureInfoReader*, const std::string&);
   void addFixtureFailure(TestFixtureInfoReader*, const AssertionFailure&);
   
   void startTestSuite(TestSuiteInfoReader*);
   void endTestSuite(TestSuiteInfoReader*);
   void addSuiteError(TestSuiteInfoReader*, const std::string&);
   
   void startTagsFiltering(const TaggableObjFilter*);
   void endTagsFiltering(const TaggableObjFilter*);

   void startTest();
   void endTest();
   void addError(const std::string&);

private:
   
   void reportSuitesResult();
   void reportCasesResult();

   void reportFailedNumber
         ( const std::string& title
         , unsigned int number);
   
   void reportCaseFailure
         ( const TestCaseInfoReader* testcase
         , unsigned int result
         , unsigned int line
         , const std::string& msg);

   void reportCaseFailure
         ( const TestCaseInfoReader* testcase
         , unsigned int result
         , const std::string& msg);
   
   void reportCaseSuccess
         ( const TestCaseInfoReader* testcase );

   void reportAllUnsuccessfulTests() const;

   void addTestResult
         ( std::list<TestCaseResult>& set
         , const TestCaseInfoReader* testcase
         , unsigned int code );

public:
   std::ostream& outputTestCaseInfo
         (std::ostream&, const TestCaseInfoReader*, bool showSuite) const;

private:   
   
   Succ    succ;
   Fail    fail;
   Info    info;
   Warning warn;
   Normal  normal;

   bool showSuite;
   bool showFixture;
   bool showTags;
   bool verbose;
   
   bool isSuccess;

   TestResultReporter*        bookKeeper;      // X
   TestSuiteResultReporter*   suiteBookKeeper; // X
   TestCaseResultReporter*    caseBookKeeper;  // X

   std::list<TestCaseResult> failedTests;
   std::list<TestCaseResult> errorTests;
   std::list<TestCaseResult> crashedTests;
   std::list<TestCaseResult> skippedTests;
};

namespace
{
   struct TestCaseInfo
   {
      TestCaseInfo
         ( const StdoutVerboseListener* listener
         , const TestCaseInfoReader* test
         , bool shouldShowSuite = false)
         : This(listener)
         , testcase(test)
         , showSuite(shouldShowSuite)
      {}

      std::ostream& output(std::ostream& os) const
      {
         return This->outputTestCaseInfo(os, testcase, showSuite);
      }

      const StdoutVerboseListener* This;
      const TestCaseInfoReader* testcase;
      bool showSuite;
   };
}

///////////////////////////////////////////////////////////
std::ostream& operator<<
      ( std::ostream& os
      , const TestCaseInfo& testcase)
{
   return testcase.output(os);
}
///////////////////////////////////////////////////////////
StdoutVerboseListener::
StdoutVerboseListener
      ( bool isColorful
      , bool shouldShowSuite
      , bool shouldShowFixture
      , bool shouldShowTags
      , bool isVerbose
      , TestResultReporter* reporter
      , TestSuiteResultReporter* suiteReporter 
      , TestCaseResultReporter* caseReporter)
: succ(isColorful)
, fail(isColorful)
, info(isColorful)
, warn(isColorful)
, normal(isColorful)
, showSuite(shouldShowSuite)
, showFixture(shouldShowFixture)
, showTags(shouldShowTags)
, verbose(isVerbose)
, isSuccess(true)
, bookKeeper(reporter)
, suiteBookKeeper(suiteReporter)
, caseBookKeeper(caseReporter)
{
}



///////////////////////////////////////////////////////////
std::ostream&
StdoutVerboseListener::
outputTestCaseInfo
      ( std::ostream& os
      , const TestCaseInfoReader* testcase
      , bool showSuite) const
{
   if(!showFixture)
   {
      if(showSuite)
      {
         os << testcase->getNameOfSuite()
            << "::";
      }

      os << testcase->getNameOfFixture()
         << "::";
   }

   os << testcase->getName();

   return os;
}
///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
reportCaseFailure
      ( const TestCaseInfoReader* testcase
      , unsigned int result
      , unsigned int line
      , const std::string& msg)
{
   if(isSuccess)
   {
      std::cout << std::endl;
      isSuccess = false;
   }

   std::cout
      << fail
      << getTitle(result)
      << normal;

   if(!verbose)
   {
      std::cout
         << info
         << TestCaseInfo(this, testcase)
         << normal
         << " - ";
   }

   std::cout 
      << testcase->getFileName()
      << ":"
      << line
      << ": " 
      << msg
      << std::endl;   
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
reportCaseSuccess
      ( const TestCaseInfoReader* testcase )
{
   isSuccess = true;

   if(verbose)
   {
      std::cout << succ << getTitle("OK") << normal << std::endl;
   }
   else
   {
      std::cout << succ << "." << normal;
      std::cout.flush();
   }
}
///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
reportCaseFailure
      ( const TestCaseInfoReader* testcase
      , unsigned int result
      , const std::string& msg)
{
   reportCaseFailure(testcase, result, testcase->getLineOfFile(), msg);
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
addCaseCrash(const TestCaseInfoReader* testcase)
{
   reportCaseFailure
      ( testcase
      , TestCaseResultReporter::TR_CRASHED
      , "test crashed unexpectedly.");
}

///////////////////////////////////////////////////////////
void 
StdoutVerboseListener::
addCaseSkipped(const TestCaseInfoReader* testcase)
{
   reportCaseFailure
      ( testcase 
      , TestCaseResultReporter::TR_SKIPPED
      , "test was skipped due to the failure of its dependent case.");
}

///////////////////////////////////////////////////////////
void 
StdoutVerboseListener::
addCaseError(const TestCaseInfoReader* testcase, const std::string& error)
{
   reportCaseFailure
      ( testcase
      , TestCaseResultReporter::TR_ERROR
      , error);
}

///////////////////////////////////////////////////////////
void 
StdoutVerboseListener::
addCaseFailure
   ( const TestCaseInfoReader* testcase
   , const AssertionFailure& failure)
{
   reportCaseFailure
      ( testcase
      , TestCaseResultReporter::TR_FAILED
      , failure.getLineOfFile()
      , failure.what());
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
startTestCase(const TestCaseInfoReader* testcase)
{
   if(!verbose) return;

   std::cout << succ << getTitle("RUN") << normal 
             << info << TestCaseInfo(this, testcase) << normal
             << std::endl;
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
addTestResult
   ( std::list<TestCaseResult>& set
   , const TestCaseInfoReader* testcase
   , unsigned int code )
{
   if(!verbose) return;

   set.push_back(TestCaseResult(testcase, code));
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
endTestCase
   ( const TestCaseInfoReader* testcase )
{
   unsigned int result = \
      caseBookKeeper->getTestCaseResult(testcase);

   switch (result) 
   {
      case TestCaseResultReporter::TR_SUCCESS:
         reportCaseSuccess(testcase);
         break;
      case TestCaseResultReporter::TR_FAILED:
      case TestCaseResultReporter::TR_ERROR:
      case TestCaseResultReporter::TR_CRASHED:   
         addTestResult(failedTests, testcase, result);
         break;
      case TestCaseResultReporter::TR_SKIPPED:
         addTestResult(skippedTests, testcase, result);
         break;
      case TestCaseResultReporter::TR_UNKNOWN:
         throw Error(TESTNGPP_INTERNAL_ERROR(3001));
         break;
   }  
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
startTestFixture(TestFixtureInfoReader* fixture)
{
   if(!showFixture) return;
   
   std::cout << std::endl;
   std::cout << info << "(" << fixture->getName() << ")" << normal;
   std::cout << std::endl;
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
endTestFixture(TestFixtureInfoReader*)
{
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
addFixtureError(TestFixtureInfoReader*, const std::string&)
{
}

///////////////////////////////////////////////////////////
void 
StdoutVerboseListener::
addFixtureFailure(TestFixtureInfoReader*, const AssertionFailure&)
{
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
startTestSuite(TestSuiteInfoReader* suite)
{
   if(!showSuite) return;
   
   std::cout << std::endl;
   std::cout
      << info 
      << formatTitle("Suite:" + suite->getName(), '-', maxLenOfLine)
      << normal
      << std::endl;
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
endTestSuite(TestSuiteInfoReader*)
{
}

///////////////////////////////////////////////////////////
void 
StdoutVerboseListener::
addSuiteError(TestSuiteInfoReader*, const std::string&)
{
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
startTagsFiltering(const TaggableObjFilter* filter)
{
   if(!showTags) return;

   std::cout << std::endl;
   std::cout
      << info 
      << formatTitle("Tags:{" + filter->toString() + "}",'=', maxLenOfLine)
      << normal 
      << std::endl;
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
endTagsFiltering(const TaggableObjFilter*)
{
}

///////////////////////////////////////////////////////////
void 
StdoutVerboseListener::
startTest()
{
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::     
reportFailedNumber(const std::string& title, unsigned int number)
{
   if(number > 0)
   {
      std::cout 
          << " " << title << ": " 
          << fail << number << normal;
   }
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::     
reportCasesResult()
{
   if(bookKeeper->getNumberOfUnsuccessfulTestCases() > 0)
   {
      unsigned int successCases = bookKeeper->getNumberOfSuccessfulTestCases();
      unsigned int rate = int(successCases*100/bookKeeper->getNumberOfTestCases()); 
      
      std::cout << std::endl;
      std::cout << " success: " << succ << successCases << normal;
      reportFailedNumber("failed", bookKeeper->getNumberOfFailedTestCases());
      reportFailedNumber("errors", bookKeeper->getNumberOfErrorTestCases());
      reportFailedNumber("crashed", bookKeeper->getNumberOfCrashedTestCases());
      reportFailedNumber("skipped", bookKeeper->getNumberOfSkippedTestCases());
      std::cout << std::endl; 
      std::cout << " success rate: " << fail << rate << '%' << normal;
      std::cout << std::endl;
   }
   
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::     
reportSuitesResult()
{
   if(bookKeeper->getNumberOfUnloadableSuites() == 0)
   {
      return;
   }
     
   unsigned int totalSuites = 
      bookKeeper->getNumberOfLoadedSuites() +
      bookKeeper->getNumberOfUnloadableSuites();
     
   unsigned int rate = 
      (unsigned int)(100*bookKeeper->getNumberOfLoadedSuites()/totalSuites);
     
   std::cout 
      << " loaded suites: " 
      << succ 
      << bookKeeper->getNumberOfLoadedSuites()
      << normal
      << " unloadable suites: " 
      << fail
      << bookKeeper->getNumberOfUnloadableSuites()
      << normal
      << std::endl
      << " load success rate: " 
      << fail
      << rate << "%"
      << normal
      << std::endl;     
}
     
void
StdoutVerboseListener::
reportAllUnsuccessfulTests() const
{
   if(!verbose) return;

   std::list<TestCaseResult>::const_iterator i = \
     failedTests.begin();

   for(; i != failedTests.end(); i++)
   {
      std::cout
         << fail << (*i).toString()
         << normal << std::endl;
   }

   for(i = skippedTests.begin(); i != skippedTests.end(); i++)
   {
      std::cout
         << warn << (*i).toString()
         << normal << std::endl;
   }
}
///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
endTest()
{
   if(bookKeeper->getNumberOfTestCases() == 0)
   {
      std::cout << fail << "no test to run!" << normal;
   }
   
   std::cout 
      << std::endl << std::endl
      << info 
      << formatTitle("RESULT", '=', maxLenOfLine)
      << normal 
      << std::endl;
     
   reportAllUnsuccessfulTests();

   if( ( bookKeeper->getNumberOfUnloadableSuites() 
       + bookKeeper->getNumberOfUnsuccessfulTestCases()) == 0)
   {
      std::cout
         << succ
         << getTitle(TestCaseResultReporter::TR_SUCCESS)
         << bookKeeper->getNumberOfTestCases()
         << normal
         << " cases from "
         << succ
         << bookKeeper->getNumberOfLoadedSuites()
         << normal
         << " suites ran successfully."
         << std::endl;
   }
     
   reportSuitesResult();

   reportCasesResult();

   std::cout << std::endl;
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
addError(const std::string&)
{
}

///////////////////////////////////////////////////////////
TESTNGPP_NS_END

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
USING_TESTNGPP_NS

#define LISTENER(name) testngppverboselistener_##name

///////////////////////////////////////////////////////////
extern "C" DLL_EXPORT
TestListener*
LISTENER(create_instance)
     ( TestResultReporter* resultReporter
     , TestSuiteResultReporter* suiteReporter
     , TestCaseResultReporter* caseResultReporter
     , int argc
     , char** argv)
{
   OptionList options;
   
   options.parse(argc, argv, "cvsft");
   
   return new StdoutVerboseListener
         ( options.hasOption("c")
         , options.hasOption("s")
         , options.hasOption("f")
         , options.hasOption("t")
         , options.hasOption("v")
         , resultReporter
         , suiteReporter
         , caseResultReporter);
}

///////////////////////////////////////////////////////////
extern "C" DLL_EXPORT
void
LISTENER(destroy_instance)(TestListener* instance)
{
   delete instance;
}

///////////////////////////////////////////////////////////
