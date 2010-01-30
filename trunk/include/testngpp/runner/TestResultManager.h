#ifndef __TESTNGPP_TEST_RESULT_MANAGER_H__
#define __TESTNGPP_TEST_RESULT_MANAGER_H__

#include <testngpp/testngpp.h>
#include <testngpp/runner/StringList.h>

TESTNGPP_NS_START

struct TestResultManager
{
   virtual void load
         ( const StringList& searchingPaths
         , const StringList& clOfListners) = 0;

   virtual bool hasFailure() const = 0;

   virtual ~TestResultManager() {}
};

TESTNGPP_NS_END

#endif

