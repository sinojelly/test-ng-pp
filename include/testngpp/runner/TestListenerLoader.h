
#ifndef __TESTNGPP_TEST_LISTENER_LOADER_H
#define __TESTNGPP_TEST_LISTENER_LOADER_H

#include <list>
#include <string>

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestListener;
struct TestRunnerContext;

struct TestListenerLoader
{
   virtual
   void load( TestRunnerContext* context
            , const std::list<std::string>& searchingPaths) = 0;

	virtual ~TestListenerLoader() {}
};

TESTNGPP_NS_END

#endif

