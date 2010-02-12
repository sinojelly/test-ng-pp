
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

   std::string
   getTitle(const std::string title)
   {
      const unsigned int maxLenOfTitle = 10;

      unsigned int numberOfBlanks = maxLenOfTitle - title.size();
      if(title.size() > maxLenOfTitle)
      {
         numberOfBlanks = 0;
         // TODO: Truncate
      }
  

      std::string leftBlanks("[");
      for(unsigned int i=0; i<numberOfBlanks/2; i++)
      {
         leftBlanks += " ";
      }

      std::string rightBlanks("]");
      for(unsigned int i=0; i<numberOfBlanks/2+numberOfBlanks%2; i++)
      {
         rightBlanks = " " + rightBlanks;
      }

      return leftBlanks + title + rightBlanks;
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
          return getTitle(result) + " " + suite + "::" + fixture + "::" + name;
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
         , const std::string& type
         , unsigned int line
         , const std::string& msg);

   void reportCaseFailure
         ( const TestCaseInfoReader* testcase
         , const std::string& type
         , const std::string& msg);
   
   void reportCaseSuccess
         ( const TestCaseInfoReader* testcase );

   void reportAllUnsuccessfulTests() const;

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
   bool failedOnly;
   
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
      , bool showFailedOnly
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
, failedOnly(showFailedOnly)
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
      , const std::string& title 
      , unsigned int line
      , const std::string& msg)
{
   std::cout
      << fail
      << getTitle(title)
      << normal
      << " ";

   if(failedOnly)
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
   if(failedOnly)
      return;

   std::cout << succ << getTitle("OK") << normal << std::endl;
}
///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
reportCaseFailure
      ( const TestCaseInfoReader* testcase
      , const std::string& type 
      , const std::string& msg)
{
   reportCaseFailure(testcase, type, testcase->getLineOfFile(), msg);
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
addCaseCrash(const TestCaseInfoReader* testcase)
{
   reportCaseFailure(testcase, "CRASHED", "test crashed unexpectedly.");
}

///////////////////////////////////////////////////////////
void 
StdoutVerboseListener::
addCaseSkipped(const TestCaseInfoReader* testcase)
{
   reportCaseFailure(testcase, "SKIPPED",
      "test was skipped due to the failure of its dependent case.");
}

///////////////////////////////////////////////////////////
void 
StdoutVerboseListener::
addCaseError(const TestCaseInfoReader* testcase, const std::string& error)
{
   reportCaseFailure(testcase, "ERROR", error);
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
      , "FAILED"
      , failure.getLineOfFile()
      , failure.what());
}

///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
startTestCase(const TestCaseInfoReader* testcase)
{
   if(failedOnly) return;

   std::cout << succ << getTitle("RUN") << normal 
             << " "
             << info << TestCaseInfo(this, testcase) << normal
             << std::endl;
}

namespace
{
   void addTestResult
      ( std::list<TestCaseResult>& set
      , const TestCaseInfoReader* testcase
      , unsigned int code )
   {
      set.push_back(TestCaseResult(testcase, code));
   }

}
///////////////////////////////////////////////////////////
void
StdoutVerboseListener::
endTestCase(const TestCaseInfoReader* testcase)
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
   std::cout << info << "[" << suite->getName() << "]" << normal;
   std::cout << std::endl;
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
   std::cout << info << "{ " << filter->toString() << " }" << normal;
   std::cout << std::endl;
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
   if(failedOnly) return;

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
      << "====================RESULT===================="
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
   
   options.parse(argc, argv, "cnsft");
   
   return new StdoutVerboseListener
         ( options.hasOption("c")
         , options.hasOption("s")
         , options.hasOption("f")
         , options.hasOption("t")
         , options.hasOption("n")
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
