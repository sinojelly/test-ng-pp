
#include <iostream>
#include <string>

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include <testngpp/Error.h>
#include <testngpp/AssertionFailure.h>

#include <testngpp/utils/OptionList.h>
#include <testngpp/utils/StringList.h>

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
   ////////////////////////////////////////////////////////
   struct OutputState
   {
   protected:
      OutputState(bool c)
         : colorful(c)
      {}
      
   public:
      bool isColorful() const
      {
         return colorful;
      }
      
   private:
      bool colorful;
   };
   
   ////////////////////////////////////////////////////////
   struct Succ : public OutputState
   {
      Succ(bool colorful) : OutputState(colorful)
      {}
   };
   
   ////////////////////////////////////////////////////////
   struct Fail : public OutputState
   {
      Fail(bool colorful) : OutputState(colorful)
      {}
   };
   
   ////////////////////////////////////////////////////////
   struct Normal : public OutputState
   {
      Normal(bool colorful) : OutputState(colorful)
      {}
   };
   
   ////////////////////////////////////////////////////////
   struct Info : public OutputState
   {
      Info(bool colorful) : OutputState(colorful)
      {}
   };
   
   ////////////////////////////////////////////////////////
   struct Warning : public OutputState
   {
      Warning(bool colorful) : OutputState(colorful)
      {}
   };

   ////////////////////////////////////////////////////////
   std::ostream& switchToFail(std::ostream& os, bool colorful)
   {
      if(colorful)
      {
   #ifdef _MSC_VER
         ::SetConsoleTextAttribute
            ( ::GetStdHandle(STD_OUTPUT_HANDLE)
            , FOREGROUND_RED | FOREGROUND_INTENSITY);
   #else
         os << "\033[1;31m";
   #endif
      }
      return os;
   }
   
   ////////////////////////////////////////////////////////
   std::ostream& switchToSucc(std::ostream& os, bool colorful)
   {
      if(colorful)
      {
   #ifdef _MSC_VER
         ::SetConsoleTextAttribute
            ( ::GetStdHandle(STD_OUTPUT_HANDLE)
            , FOREGROUND_GREEN | FOREGROUND_INTENSITY);
   #else
         os << "\033[1;32m";
   #endif
      }
      return os;
   } 
   
   ////////////////////////////////////////////////////////
   std::ostream& switchToNorm(std::ostream& os, bool colorful)
   {
      if(colorful)
      {
   #ifdef _MSC_VER
         ::SetConsoleTextAttribute
            ( ::GetStdHandle(STD_OUTPUT_HANDLE)
            , FOREGROUND_INTENSITY);
   #else
         os << "\033[0m";
   #endif
      }
      return os;
   }
   
   ////////////////////////////////////////////////////////
   std::ostream& switchToInfo(std::ostream& os, bool colorful)
   {
      if(colorful)
      {
   #ifdef _MSC_VER
         ::SetConsoleTextAttribute
         ( ::GetStdHandle(STD_OUTPUT_HANDLE)
          , FOREGROUND_INTENSITY);
   #else
         os << "\33[1;36m";
   #endif
      }
      return os;
   }

   ////////////////////////////////////////////////////////
   std::ostream& switchToWarning(std::ostream& os, bool colorful)
   {
      if(colorful)
      {
   #ifdef _MSC_VER
         ::SetConsoleTextAttribute
         ( ::GetStdHandle(STD_OUTPUT_HANDLE)
          , FOREGROUND_INTENSITY);
   #else
         os << "\33[1;35m";
   #endif
      }
      return os;
   }
}

///////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Succ& succ)
{
   return switchToSucc(os, succ.isColorful());
}

///////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Fail& fail)
{
   return switchToFail(os, fail.isColorful());
}

///////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Normal& norm)
{
   return switchToNorm(os, norm.isColorful());
}

///////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Info& info)
{
   return switchToInfo(os, info.isColorful());
}

///////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Warning& warn)
{
   return switchToWarning(os, warn.isColorful());
}

///////////////////////////////////////////////////////////
struct StdoutListener : public TestListener
{
   StdoutListener
      ( bool 
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
         , unsigned int line
         , const std::string& msg);

   void reportCaseFailure
         ( const TestCaseInfoReader* testcase
         , const std::string& msg);
   
private:   
   
   Succ    succ;
   Fail    fail;
   Info    info;
   Warning warn;
   Normal  normal;

   bool showSuite;
   bool showFixture;
   bool showTags;
   
   TestResultReporter*        bookKeeper;      // X
   TestSuiteResultReporter*   suiteBookKeeper; // X
   TestCaseResultReporter*    caseBookKeeper;  // X
};



///////////////////////////////////////////////////////////
StdoutListener::
StdoutListener
      ( bool isColorful
      , bool shouldShowSuite
      , bool shouldShowFixture
      , bool shouldShowTags
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
, bookKeeper(reporter)
, suiteBookKeeper(suiteReporter)
, caseBookKeeper(caseReporter)
{
}

///////////////////////////////////////////////////////////
void
StdoutListener::
reportCaseFailure
      ( const TestCaseInfoReader* testcase
      , unsigned int line
      , const std::string& msg)
{
   std::cerr
      << std::endl
      << warn;

   if(!showFixture)
   {
      std::cerr
         << testcase->getNameOfFixture()
         << "::";
   }

   std::cerr
      << testcase->getName() 
      << normal
      << " - "
      << testcase->getFileName()
      << ":"
      << line
      << ": " 
      << msg
      << std::endl;   
}

///////////////////////////////////////////////////////////
void
StdoutListener::
reportCaseFailure
      ( const TestCaseInfoReader* testcase
      , const std::string& msg)
{
   reportCaseFailure(testcase, testcase->getLineOfFile(), msg);
}

///////////////////////////////////////////////////////////
void
StdoutListener::
addCaseCrash(const TestCaseInfoReader* testcase)
{
   reportCaseFailure(testcase, "test crashed unexpectedly.");
}

///////////////////////////////////////////////////////////
void 
StdoutListener::
addCaseSkipped(const TestCaseInfoReader* testcase)
{
   reportCaseFailure(testcase, 
      "test was skipped due to the failure of its dependent case.");
}

///////////////////////////////////////////////////////////
void 
StdoutListener::
addCaseError(const TestCaseInfoReader* testcase, const std::string& error)
{
   reportCaseFailure(testcase, error);
}

///////////////////////////////////////////////////////////
void 
StdoutListener::
addCaseFailure
   ( const TestCaseInfoReader* testcase
   , const AssertionFailure& failure)
{
   reportCaseFailure
      ( testcase
      , failure.getLineOfFile()
      , failure.what());
}

///////////////////////////////////////////////////////////
void
StdoutListener::
startTestCase(const TestCaseInfoReader* testcase)
{
}

///////////////////////////////////////////////////////////
void
StdoutListener::
endTestCase(const TestCaseInfoReader* testcase)
{
   switch (caseBookKeeper->getTestCaseResult(testcase)) 
   {
      case TestCaseResultReporter::TR_SUCCESS:
         std::cout << succ << "." << normal;
         break;
      case TestCaseResultReporter::TR_FAILED:
         //std::cout << fail << "F" << normal;   
         break;
      case TestCaseResultReporter::TR_ERROR:
         //std::cout << fail << "E" << normal;
         break;
      case TestCaseResultReporter::TR_SKIPPED:
         //std::cout << fail << "S" << normal;
         break;
      case TestCaseResultReporter::TR_CRASHED:   
         //std::cout << fail << "C" << normal;
         break;
      case TestCaseResultReporter::TR_UNKNOWN:
         throw Error(TESTNGPP_INTERNAL_ERROR(3001));
         break;
      default:
         break;
   }  
   std::cout.flush();
}

///////////////////////////////////////////////////////////
void
StdoutListener::
startTestFixture(TestFixtureInfoReader* fixture)
{
   if(!showFixture) return;
   
   std::cout << std::endl;
   std::cout << info << "(" << fixture->getName() << ")" << normal;
   std::cout << std::endl;
}

///////////////////////////////////////////////////////////
void
StdoutListener::
endTestFixture(TestFixtureInfoReader*)
{
}

///////////////////////////////////////////////////////////
void
StdoutListener::
addFixtureError(TestFixtureInfoReader*, const std::string&)
{
}

///////////////////////////////////////////////////////////
void 
StdoutListener::
addFixtureFailure(TestFixtureInfoReader*, const AssertionFailure&)
{
}

///////////////////////////////////////////////////////////
void
StdoutListener::
startTestSuite(TestSuiteInfoReader* suite)
{
   if(!showSuite) return;
   
   std::cout << std::endl;
   std::cout << info << "[" << suite->getName() << "]" << normal;
   std::cout << std::endl;
}

///////////////////////////////////////////////////////////
void
StdoutListener::
endTestSuite(TestSuiteInfoReader*)
{
}

///////////////////////////////////////////////////////////
void 
StdoutListener::
addSuiteError(TestSuiteInfoReader*, const std::string&)
{
}

///////////////////////////////////////////////////////////
void
StdoutListener::
startTagsFiltering(const TaggableObjFilter* filter)
{
   if(!showTags) return;

   std::cout
      << std::endl << std::endl
      << info << "{ " << filter->toString() << " }" 
      << normal << std::endl;
}

///////////////////////////////////////////////////////////
void
StdoutListener::
endTagsFiltering(const TaggableObjFilter*)
{
}

///////////////////////////////////////////////////////////
void 
StdoutListener::
startTest()
{
}

///////////////////////////////////////////////////////////
void
StdoutListener::     
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
StdoutListener::     
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
      
      std::cout 
         << " success rate: " << fail << rate << '%' << normal 
         << std::endl;
   }
   
}

///////////////////////////////////////////////////////////
void
StdoutListener::     
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
     
///////////////////////////////////////////////////////////
void
StdoutListener::
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
     
   if( ( bookKeeper->getNumberOfUnloadableSuites() 
       + bookKeeper->getNumberOfUnsuccessfulTestCases()) == 0)
   {
      std::cout
         << succ
         << "[  OK  ] "
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
StdoutListener::
addError(const std::string&)
{
}

///////////////////////////////////////////////////////////
TESTNGPP_NS_END

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
USING_TESTNGPP_NS

#define LISTENER(name) testngppstdoutlistener_##name

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
   
   options.parse(argc, argv, "cfst");
   
   return new StdoutListener
         ( options.hasOption("c")
         , options.hasOption("s")
         , options.hasOption("f")
         , options.hasOption("t")
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
