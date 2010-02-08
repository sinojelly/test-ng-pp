
#include <string>
#include <iostream>

#include <windows.h>

#include <testngpp/comm/Win32PipeWrittableChannel.h>
#include <testngpp/runner/TestCaseSandboxResultReporter.h>
#include <testngpp/runner/ModuleTestSuiteLoaderFactory.h>
#include <testngpp/runner/TestFilter.h>
#include <testngpp/runner/TestFilterFactory.h>
#include <testngpp/runner/TagsFilters.h>
#include <testngpp/runner/TagsParser.h>
#include <testngpp/runner/TestRunnerContext.h>
#include <testngpp/runner/TestSuiteRunner.h>
#include <testngpp/runner/TestFixtureRunner.h>
#include <testngpp/runner/TestFixtureRunnerFactory.h>
#include <testngpp/win32/Win32TestCaseRunnerResultReporter.h>

void usage()
{
    std::cerr << "Usage: testngpp-win32-testcase-runner.exe testsuite fixture testcase" << std::endl;
	ExitProcess(1);
}

TESTNGPP_NS::TestCaseRunnerResultReporter* 
createCollector(HANDLE hWrite, HANDLE hSemaphore)
{
   if(hWrite == 0)
   {
	   hWrite = GetStdHandle(STD_OUTPUT_HANDLE);  
	   if (hWrite == INVALID_HANDLE_VALUE)
	   {		
		  ExitProcess(1); 
	   }
   }
   
   return new TESTNGPP_NS::TestCaseRunnerResultReporter(hSemaphore,
	         new TESTNGPP_NS::TestCaseSandboxResultReporter(
	            new TESTNGPP_NS::Win32PipeWrittableChannel(hWrite)));
}

const TESTNGPP_NS::TestFilter*
createFilter
      ( const std::string& fixtureName
	  , const std::string& testcaseName)
{
	TESTNGPP_NS::StringList filterPattern;
	filterPattern.add(fixtureName + ":" + testcaseName);
	return TESTNGPP_NS::TestFilterFactory::getFilter(filterPattern);
}

void runTest
      ( TESTNGPP_NS::TestCaseRunnerResultReporter* collector
      , const std::string& suiteName
	  , const std::string& fixtureName
	  , const std::string& testcaseName)
{
	const TESTNGPP_NS::TestFilter* filter = createFilter(fixtureName, testcaseName);
	TESTNGPP_NS::TagsFilters* tagsFilter = TESTNGPP_NS::TagsParser::parse("*");

	TESTNGPP_NS::StringList suites;
	suites.add(suiteName);

	try {
	TESTNGPP_NS::TestRunnerContext* context = \
		new TESTNGPP_NS::TestRunnerContext
            ( suites
            , collector
            , tagsFilter
            , filter);

	TESTNGPP_NS::TestFixtureRunner* fixtureRunner = 
		TESTNGPP_NS::TestFixtureRunnerFactory::createInstance(false, 1);

	TESTNGPP_NS::TestSuiteRunner* suiteRunner = \
		new TESTNGPP_NS::TestSuiteRunner(fixtureRunner, collector);

	if(!tagsFilter->startOnNext())
	{
		std::cerr << "internal error" << std::endl;
	}
	////////////////////////////////////////////////////
    suiteRunner->run(context->getSuite(0), filter);
	////////////////////////////////////////////////////

	delete suiteRunner;
	delete fixtureRunner;
	delete tagsFilter;
	}
	catch(...)
	{
		std::cerr << "exception" << std::endl;
	}

	TESTNGPP_NS::TestFilterFactory::returnFilter(filter);
}

int main(int argc, char* argv[])
{
   if(argc != 6 && argc != 4)
   {
      usage(); 
   }
   
   std::cout << "start to run " << std::endl;
   std::string suite(argv[1]);
   std::string fixture(argv[2]);
   std::string testcase(argv[3]);
   
   HANDLE hWrite = 0;
   HANDLE hSemap = 0;
   
   if(argc == 6)
   {
      hWrite = (HANDLE)atoi(argv[4]);
      hSemap = (HANDLE)atoi(argv[5]);
   }

   runTest(createCollector(hWrite, hSemap), suite, fixture, testcase);
	
   return 0;
}