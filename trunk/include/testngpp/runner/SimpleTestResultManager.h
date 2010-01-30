#ifndef __TESTNGPP_SIMPLE_TEST_RESULT_MANAGER_H__
#define __TESTNGPP_SIMPLE_TEST_RESULT_MANAGER_H__

#include <testngpp/testngpp.h>

#include <testngpp/runner/TestResultManager.h>

TESTNGPP_NS_START

struct SimpleTestResultManagerImpl;

struct SimpleTestResultManager : public TestResultManager
{
   void load( const StringList& searchingPaths
            , const StringList& clOfListners) = 0;

   bool hasFailure() const;

   SimpleTestResultManager(TestListenerLoaderFactory* );
   ~SimpleTestResultManager();

private:
   SimpleTestResultManagerImpl* This;

};

TESTNGPP_NS_END

#endif

