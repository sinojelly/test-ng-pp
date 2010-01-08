/**
    CxxTestPlus is a practical, easy-to-use C/C++ xUnit framework.
    Copyright (C) <2009>  <Arthur Yuan: arthur.ii.yuan@gmail.com>

    CxxTestPlus is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CxxTestPlus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CxxTestPlus.  If not, see <http://www.gnu.org/licenses/>.
**/

#include <iostream>
#include <string.h>

#include <testcpp/Error.h>

#include <testcpp/internal/TestCaseInfoReader.h>
#include <testcpp/internal/TestSuiteInfoReader.h>

#include <testcpp/runner/TestResultReporter.h>
#include <testcpp/runner/TestSuiteResultReporter.h>
#include <testcpp/runner/TestCaseResultReporter.h>
#include <testcpp/runner/InternalError.h>

#include <StdoutTestListener.h>
#include <ColorfulStdoutTestListener.h>

TESTCPP_NS_START

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
addCaseCrash(TestCaseInfoReader* testcase)
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
addCaseError(TestCaseInfoReader* testcase, const std::string& msg)
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
addCaseFailure(TestCaseInfoReader* testcase, const AssertionFailure& failure)
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
startTestCase(TestCaseInfoReader*)
{
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
endTestCase(TestCaseInfoReader* testcase)
{
   switch(This->caseResultReporter->getTestCaseResult(testcase))
   {
   case TestCaseResultReporter::SUCCESS:
      std::cout << "."; 
      break;
   case TestCaseResultReporter::FAILED:
      std::cout << "F";
      break;
   case TestCaseResultReporter::ERROR:
      std::cout << "E"; 
      break;
   case TestCaseResultReporter::UNKNOWN:
      throw Error(TESTCPP_INTERNAL_ERROR(3001));
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
#if 0
   std::cout << suite->getName() << std::endl;
#endif
}

///////////////////////////////////////////////////////////
void StdoutTestListener::
endTestSuite(TestSuiteInfoReader* suite)
{
#if 0
   if(suite == 0)
   {
      return;
   }

   TestSuiteResultReporter* reporter = This->suiteResultReporter;

   unsigned int successCases = reporter->getNumberOfSuccessfulTestCases(suite);

   int successRate = 0;
   if(reporter->getNumberOfTestCases(suite) != 0)
   {
      successRate = int(successCases*100/reporter->getNumberOfTestCases(suite)); 
   }

   std::cout << std::endl
             << " success: " << successCases
             << " failed: " << reporter->getNumberOfFailedTestCases(suite)
             << " error: "  << reporter->getNumberOfErrorTestCases(suite)
             << " crash: "  << reporter->getNumberOfCrashedTestCases(suite) << std::endl
             << " success rate: " << successRate << "%"
             << std::endl
             << "======================================"
             << std::endl;
#endif

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
   {}

   TestResultReporter* resultReporter;
   TestSuiteResultReporter* suiteResultReporter;
   TestCaseResultReporter* caseResultReporter;
};

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
addCaseCrash(TestCaseInfoReader* testcase)
{
   std::cout << "\33[31m[ CRASHED ]\33[0m"
             << testcase->getFileName() 
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
addCaseError(TestCaseInfoReader* testcase, const std::string& msg)
{
   std::cout << "\33[31m[   ERROR ]\33[0m"
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
void ColorfulStdoutTestListener::
addCaseFailure(TestCaseInfoReader* testcase, const AssertionFailure& failure)
{
   std::cout << "\33[31m[  FAILED ]\33[0m"
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
void ColorfulStdoutTestListener::
startTestCase(TestCaseInfoReader* testcase)
{  
      std::cout << "\33[32m[ RUN     ]\33[0m"
	            << testcase->getName()
				<< std::endl; 
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
endTestCase(TestCaseInfoReader* testcase)
{
   switch(This->caseResultReporter->getTestCaseResult(testcase))
   {
   case TestCaseResultReporter::SUCCESS:
      std::cout << "\33[32m[      OK ]\33[0m"
	            << testcase->getName()
				<< std::endl; 
      break;
   case TestCaseResultReporter::FAILED:
      break;
   case TestCaseResultReporter::ERROR:
      break;
   case TestCaseResultReporter::UNKNOWN:
      throw Error(TESTCPP_INTERNAL_ERROR(3001));
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
   std::cout << "\33[32m[ FIXTURE ERROR ]\33[0m"
             << msg << std::endl;
}

///////////////////////////////////////////////////////////
void ColorfulStdoutTestListener::
addFixtureFailure(TestFixtureInfoReader*, const AssertionFailure& failure)
{
   std::cout << "\33[32m[ FIXTURE FAILURE ]\33[0m"
             << failure.what() << std::endl;
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
   std::cout << "\33[32m[ SUITE FAILURE ]\33[0m"
             << msg
             << std::endl;
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
      std::cout << std::endl 
                << " success rate: " << rate << '%' << std::endl;
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

TESTCPP_NS_END

///////////////////////////////////////////////////////////
USING_TESTCPP_NS
///////////////////////////////////////////////////////////
extern "C"
TestListener*
testcppstdoutlistener_create_instance(
   TestResultReporter* resultReporter,
   TestSuiteResultReporter* suiteReporter,
   TestCaseResultReporter* caseResultReporter,
   int argc, char**argv)
{
    if(argc == 2 && !strcmp("-c", argv[1]))
        return new ColorfulStdoutTestListener(resultReporter, suiteReporter, caseResultReporter);

     return new StdoutTestListener(resultReporter, suiteReporter, caseResultReporter);
}

///////////////////////////////////////////////////////////
extern "C"
void
testcppstdoutlistener_destroy_instance(TestListener* instance)
{
    delete instance;
}

///////////////////////////////////////////////////////////
