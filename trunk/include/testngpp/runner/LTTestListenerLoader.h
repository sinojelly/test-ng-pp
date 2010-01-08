
#ifndef __TESTNGPP_LT_TEST_LISTENER_LOADER_H
#define __TESTNGPP_LT_TEST_LISTENER_LOADER_H

#include <list>

#include <testngpp/testngpp.h>

#include <testngpp/runner/TestListenerLoader.h>

TESTNGPP_NS_START

struct TestListener;
struct TestRunnerContext;
struct LTTestListenerLoaderImpl;

struct LTTestListenerLoader : public TestListenerLoader
{
   LTTestListenerLoader(const std::string& name);
   ~LTTestListenerLoader();

   void load( TestRunnerContext* context
            , const std::list<std::string>& searchingPaths);
private:
   LTTestListenerLoaderImpl* This;

};

TESTNGPP_NS_END

#endif

