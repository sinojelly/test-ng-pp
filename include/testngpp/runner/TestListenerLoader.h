
#ifndef __TESTNGPP_TEST_LISTENER_LOADER_H
#define __TESTNGPP_TEST_LISTENER_LOADER_H

#include <string>

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestListener;
struct TestResultReporter;
struct TestSuiteResultReporter;
struct TestCaseResultReporter;

struct TestListenerLoader
{
   virtual
   TestListener*
   load( const std::string& commandLine
       , TestResultReporter* resultReporter
       , TestSuiteResultReporter* suiteResultReporter
       , TestCaseResultReporter* caseResultReporter) = 0;

	virtual ~TestListenerLoader() {}
};

TESTNGPP_NS_END

#endif

