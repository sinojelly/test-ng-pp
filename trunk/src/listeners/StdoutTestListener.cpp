
#include <iostream>
#include <string.h>


#include <testngpp/Error.h>

#include <testngpp/internal/TestCaseInfoReader.h>
#include <testngpp/internal/TestSuiteInfoReader.h>

#include <testngpp/runner/TestResultReporter.h>
#include <testngpp/runner/TestSuiteResultReporter.h>
#include <testngpp/runner/TestCaseResultReporter.h>
#include <testngpp/runner/InternalError.h>

#include <StdoutTestListener.h>
#include <ColorfulStdoutTestListener.h>

#ifdef _MSC_VER
#include <windows.h>
#endif


TESTNGPP_NS_START

struct StdoutTestListenerImpl
{
   StdoutTestListenerImpl(TestResultReporter* reporter
      , TestSuiteResultReporter* suiteReporter
      , TestCaseResultReporter* caseReporter)
      : resultReporter(reporter)
      , suiteResultReporter(suiteReporter)
      , caseResultReporter(caseReporter)
   {}

   TestResultReporter* resultReporter;
   TestSuiteResultReporter* suiteResultReporter;
   TestCaseResultReporter* caseResultReporter;
};

///////////////////////////////////////////////////////////
StdoutTestListener::StdoutTestListener(TestResultReporter* reporter
   , TestSuiteResultReporter* suiteReporter
   , TestCaseResultReporter* caseReporter)
   : This(new StdoutTestListenerImpl(reporter, suiteReporter, caseReporter))
{
}

///////////////////////////////////////////////////////////
StdoutTestListener::~StdoutTestListener()
{
   delete This;
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
addCaseSkipped(const TestCaseInfoReader* testcase)
{
   std::cout << "S";
   std::cout.flush();
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
addCaseCrash(const TestCaseInfoReader* testcase)
{
   std::cout << "C";
   std::cout.flush();

   std::cerr << std::endl
             << testcase->getFileName() 
             << ":" 
             << testcase->getLineOfFile() 
             << ":" 
             << testcase->getNameOfFixture() 
             << ":" 
             << testcase->getName()
             << ": test case crashed unexpectedly." 
             << std::endl;
}
///////////////////////////////////////////////////////////
void StdoutTestListener::
addCaseError(const TestCaseInfoReader* testcase, const std::string& msg)
{
   std::cerr << std::endl
             << testcase->getFileName() 
             << ":" 
             << testcase->getLineOfFile() 
             << ":" 
             << testcase->getNameOfFixture() 
             << ":" 
             << testcase->getName()
             << ": " 
             << msg << std::endl;
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
addCaseFailure(const TestCaseInfoReader* testcase, const AssertionFailure& failure)
{
   std::cerr << std::endl
             << failure.getFileName()
             << ":"
             << failure.getLineOfFile()
             << ":" 
             << testcase->getNameOfFixture() 
             << ":" 
             << testcase->getName()
             << ": "
             << failure.what()
             << std::endl;
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
startTestCase(const TestCaseInfoReader*)
{
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
endTestCase(const TestCaseInfoReader* testcase)
{
   switch(This->caseResultReporter->getTestCaseResult(testcase))
   {
   case TestCaseResultReporter::TR_SUCCESS:
      std::cout << "."; 
      break;
   case TestCaseResultReporter::TR_FAILED:
      std::cout << "F";
      break;
   case TestCaseResultReporter::TR_ERROR:
      std::cout << "E"; 
      break;
   case TestCaseResultReporter::TR_SKIPPED:
      std::cout << "S"; 
      break;
   case TestCaseResultReporter::TR_UNKNOWN:
      throw Error(TESTNGPP_INTERNAL_ERROR(3001));
   }
   std::cout.flush();
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
startTestFixture(TestFixtureInfoReader*)
{

}

///////////////////////////////////////////////////////////
void StdoutTestListener::
endTestFixture(TestFixtureInfoReader*)
{
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
addFixtureError(TestFixtureInfoReader*, const std::string& msg)
{
   std::cerr << std::endl
             << "fixture error: " 
             << msg << std::endl;
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
addFixtureFailure(TestFixtureInfoReader*, const AssertionFailure& failure)
{
   std::cerr << std::endl
             << "fixture failure:" 
             << failure.what() << std::endl;
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
startTestSuite(TestSuiteInfoReader* suite)
{
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
endTestSuite(TestSuiteInfoReader* suite)
{
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
addSuiteError(TestSuiteInfoReader*, const std::string& msg )
{
   std::cerr << std::endl
             << "suite error: " 
             << msg
             << std::endl;
}

///////////////////////////////////////////////////////////
void StdoutTestListener::startTest()
{
}

///////////////////////////////////////////////////////////
void StdoutTestListener::endTest()
{


   if(This->resultReporter->getNumberOfTestCases() == 0)
   {
      std::cerr << "No test to run!" << std::endl;
   }


   if(This->resultReporter->getNumberOfUnloadableSuites() == 0 &&
         This->resultReporter->getNumberOfUnsuccessfulTestCases() == 0)
   {
      std::cout << "(" << This->resultReporter->getNumberOfTestCases()
                << " cases) OK!" << std::endl;
      return ;
   }



   std::cout << std::endl;

   if(This->resultReporter->getNumberOfUnloadableSuites() > 0)
   {
      unsigned int totalSuites = This->resultReporter->getNumberOfLoadedSuites() 
                               + This->resultReporter->getNumberOfUnloadableSuites();

      unsigned int rate = (unsigned int)(100*This->resultReporter->getNumberOfLoadedSuites()/totalSuites);

      std::cout << " loaded suites: " << This->resultReporter->getNumberOfLoadedSuites()
                << " unloadable suites: " << This->resultReporter->getNumberOfUnloadableSuites()
                << std::endl
                << " load success rate: " << rate << "%"
                << std::endl;
   }

   if(This->resultReporter->getNumberOfUnsuccessfulTestCases() > 0)
   {
      unsigned int successCases = This->resultReporter->getNumberOfSuccessfulTestCases();
      unsigned int rate = int(successCases*100/This->resultReporter->getNumberOfTestCases()); 

      std::cout << " success: " << successCases;

      if(This->resultReporter->getNumberOfFailedTestCases() > 0)
      {
         std::cout << " failed: " << This->resultReporter->getNumberOfFailedTestCases();
      }

      if(This->resultReporter->getNumberOfErrorTestCases() > 0)
      {
         std::cout << " error: "  << This->resultReporter->getNumberOfErrorTestCases();
      }

      if(This->resultReporter->getNumberOfCrashedTestCases() > 0)
      {
         std::cout << " crashed: "  << This->resultReporter->getNumberOfCrashedTestCases();
      }

      if(This->resultReporter->getNumberOfSkippedTestCases() > 0)
      {
         std::cout << " skipped: "  << This->resultReporter->getNumberOfSkippedTestCases();
      }

      std::cout << std::endl 
                << " success rate: " << rate << '%' << std::endl;
   }
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
addError(const std::string& msg)
{
   std::cerr << std::endl
             << "error: " 
             << msg
             << std::endl;
}

///////////////////////////////////////////////////////////

struct ColorfulStdoutTestListenerImpl
{
   ColorfulStdoutTestListenerImpl(TestResultReporter* reporter
      , TestSuiteResultReporter* suiteReporter
      , TestCaseResultReporter* caseReporter)
      : resultReporter(reporter)
      , suiteResultReporter(suiteReporter)
      , caseResultReporter(caseReporter)
   {
#ifdef _MSC_VER
	   hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE); 
#endif
   }

   ~ColorfulStdoutTestListenerImpl()
   {
   }

#ifdef _MSC_VER
   void*       hConsole;
#endif   

   void switchTextColorToSucc();
   void switchTextColorToFail();
   void restoreTextColor();

   TestResultReporter* resultReporter;
   TestSuiteResultReporter* suiteResultReporter;
   TestCaseResultReporter* caseResultReporter;

};

void ColorfulStdoutTestListenerImpl::switchTextColorToFail()
{
#ifdef _MSC_VER
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
	std::cout << "\33[31m";
#endif
}

void ColorfulStdoutTestListenerImpl::switchTextColorToSucc()
{
#ifdef _MSC_VER
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
	std::cout << "\33[32m";
#endif
}

void ColorfulStdoutTestListenerImpl::restoreTextColor()
{
#ifdef _MSC_VER
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
#else
	std::cout << "\33[0m";
#endif
}
///////////////////////////////////////////////////////////
ColorfulStdoutTestListener::ColorfulStdoutTestListener(TestResultReporter* reporter
   , TestSuiteResultReporter* suiteReporter
   , TestCaseResultReporter* caseReporter)
   : This(new ColorfulStdoutTestListenerImpl(reporter, suiteReporter, caseReporter))
{
}

///////////////////////////////////////////////////////////
ColorfulStdoutTestListener::~ColorfulStdoutTestListener()
{
   delete This;
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
addCaseCrash(const TestCaseInfoReader* testcase)
{
   This->switchTextColorToFail();
   std::cout << "[ CRASHED ]";
   This->restoreTextColor();
   std::cout << testcase->getFileName() 
             << ":" 
             << testcase->getLineOfFile() 
             << ":" 
             << testcase->getNameOfFixture() 
             << ":" 
             << testcase->getName()
             << ": Test crashed unexpectedly" 
             << std::endl;
}
///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
addCaseSkipped(const TestCaseInfoReader* testcase)
{
   This->switchTextColorToFail();
   std::cout << "[ SKIPPED ]";
   This->restoreTextColor();
   std::cout << testcase->getFileName() 
             << ":" 
             << testcase->getLineOfFile() 
             << ":" 
             << testcase->getNameOfFixture() 
             << ":" 
             << testcase->getName()
             << ": Test is skipped due to the failure of depended test case." 
             << std::endl;
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
addCaseError(const TestCaseInfoReader* testcase, const std::string& msg)
{
   This->switchTextColorToFail();
   std::cout << "[   ERROR ]";

   This->restoreTextColor();
   std::cout << testcase->getFileName() 
             << ":" 
             << testcase->getLineOfFile() 
             << ":" 
             << testcase->getNameOfFixture() 
             << ":" 
             << testcase->getName()
             << ": " 
             << msg << std::endl;
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
addCaseFailure(const TestCaseInfoReader* testcase, const AssertionFailure& failure)
{
   This->switchTextColorToFail();
   std::cout << "[  FAILED ]";
   This->restoreTextColor();

   std::cout << failure.getFileName()
             << ":"
             << failure.getLineOfFile()
             << ":" 
             << testcase->getNameOfFixture() 
             << ":" 
             << testcase->getName()
             << ": "
             << failure.what()
             << std::endl;
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
startTestCase(const TestCaseInfoReader* testcase)
{  
   This->switchTextColorToSucc();
   std::cout << "[ RUN     ]";
   This->restoreTextColor();

   std::cout << testcase->getName() << std::endl; 
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
endTestCase(const TestCaseInfoReader* testcase)
{
   switch(This->caseResultReporter->getTestCaseResult(testcase))
   {
   case TestCaseResultReporter::TR_SUCCESS:
	   This->switchTextColorToSucc();
	   std::cout << "[      OK ]" << std::endl;
      This->restoreTextColor();  
      break;
   case TestCaseResultReporter::TR_SKIPPED:
   case TestCaseResultReporter::TR_FAILED:
   case TestCaseResultReporter::TR_ERROR:
       break;
   case TestCaseResultReporter::TR_UNKNOWN:
       throw Error(TESTNGPP_INTERNAL_ERROR(3001));
   }
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
startTestFixture(TestFixtureInfoReader*)
{

}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
endTestFixture(TestFixtureInfoReader*)
{
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
addFixtureError(TestFixtureInfoReader*, const std::string& msg)
{
   This->switchTextColorToFail();
   std::cout << "[  ERROR  ]";
   This->restoreTextColor();
   std::cout << msg << std::endl;
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
addFixtureFailure(TestFixtureInfoReader*, const AssertionFailure& failure)
{
   This->switchTextColorToFail();
   std::cout << "[ FAILURE ]";
   This->restoreTextColor();
   std::cout << failure.what() << std::endl;
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
startTestSuite(TestSuiteInfoReader* suite)
{
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
endTestSuite(TestSuiteInfoReader* suite)
{
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
addSuiteError(TestSuiteInfoReader*, const std::string& msg )
{
   This->switchTextColorToFail();
   std::cout << "[ FAILURE ]";
   This->restoreTextColor();
   std::cout << msg << std::endl;
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::startTest()
{
}




///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::endTest()
{
   if(This->resultReporter->getNumberOfTestCases() == 0)
   {
      std::cerr << "No test to run!" << std::endl;
   }

   if(This->resultReporter->getNumberOfUnloadableSuites() == 0 &&
         This->resultReporter->getNumberOfUnsuccessfulTestCases() == 0)
   {
      std::cout << "(" 
                << This->resultReporter->getNumberOfTestCases()
                << " cases) ";

      This->switchTextColorToSucc();
	  std::cout << "OK!" << std::endl;
      This->restoreTextColor();          
      
      return ;
   }

   std::cout << std::endl;

   if(This->resultReporter->getNumberOfUnloadableSuites() > 0)
   {
      unsigned int totalSuites = This->resultReporter->getNumberOfLoadedSuites() 
                               + This->resultReporter->getNumberOfUnloadableSuites();

      unsigned int rate = \
          (unsigned int)(100*This->resultReporter->getNumberOfLoadedSuites()/totalSuites);

      std::cout << " loaded suites: " 
                << This->resultReporter->getNumberOfLoadedSuites()
                << " unloadable suites: " ;
	  
	  This->switchTextColorToFail();
	  std::cout << This->resultReporter->getNumberOfUnloadableSuites() << std::endl;
      This->restoreTextColor();  

      std::cout << " load success rate: " ;
      This->switchTextColorToFail();
	  std::cout << rate << "%" << std::endl;
      This->restoreTextColor();  
   }

   if(This->resultReporter->getNumberOfUnsuccessfulTestCases() > 0)
   {
      unsigned int successCases = \
          This->resultReporter->getNumberOfSuccessfulTestCases();
      unsigned int rate = \
          (unsigned int)(successCases*100/This->resultReporter->getNumberOfTestCases()); 

      std::cout << " success: ";
      This->switchTextColorToSucc();
	  std::cout << successCases;
      This->restoreTextColor();          
      
      if(This->resultReporter->getNumberOfFailedTestCases() > 0)
      {
         std::cout << " failed: ";

         This->switchTextColorToFail();
	      std::cout << This->resultReporter->getNumberOfFailedTestCases();
         This->restoreTextColor();  
      }

      if(This->resultReporter->getNumberOfErrorTestCases() > 0)
      {
		   std::cout << " error: " ;

         This->switchTextColorToFail();
	      std::cout << This->resultReporter->getNumberOfErrorTestCases();
         This->restoreTextColor();  
      }

      if(This->resultReporter->getNumberOfCrashedTestCases() > 0)
      {
         std::cout << " crashed: ";

		   This->switchTextColorToFail();
	      std::cout << This->resultReporter->getNumberOfCrashedTestCases();
         This->restoreTextColor();  
      }

      if(This->resultReporter->getNumberOfSkippedTestCases() > 0)
      {
         std::cout << " skipped: ";

		   This->switchTextColorToFail();
	      std::cout << This->resultReporter->getNumberOfSkippedTestCases();
         This->restoreTextColor();  
      }

      std::cout << std::endl << " success rate: ";
	   This->switchTextColorToFail();
      std::cout << rate << '%' << std::endl;
      This->restoreTextColor();
   }
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
addError(const std::string& msg)
{
   std::cerr << std::endl
             << "error: " 
             << msg
             << std::endl;
}

TESTNGPP_NS_END

///////////////////////////////////////////////////////////
USING_TESTNGPP_NS

#define LISTENER(name) testngppstdoutlistener_##name


///////////////////////////////////////////////////////////
extern "C" DLL_EXPORT
TestListener*
LISTENER(create_instance)(
   TestResultReporter* resultReporter,
   TestSuiteResultReporter* suiteReporter,
   TestCaseResultReporter* caseResultReporter,
   int argc, char**argv)
{
    if(argc == 2 && !strcmp("-c", argv[1]))
        return new ColorfulStdoutTestListener( \
                 resultReporter, \
                 suiteReporter, \
                 caseResultReporter);

     return new StdoutTestListener( \
                 resultReporter, \
                 suiteReporter, \
                 caseResultReporter);
}

///////////////////////////////////////////////////////////
extern "C" DLL_EXPORT
void
LISTENER(destroy_instance)(TestListener* instance)
{
    delete instance;
}

///////////////////////////////////////////////////////////
