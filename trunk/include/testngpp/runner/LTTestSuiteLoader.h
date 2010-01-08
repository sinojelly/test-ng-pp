
#ifndef __TESTNGPP_LT_TEST_SUITE_LOADER_H
#define __TESTNGPP_LT_TEST_SUITE_LOADER_H

#include <testngpp/testngpp.h>

#include <testngpp/runner/TestSuiteLoader.h>

TESTNGPP_NS_START

struct TestSuiteDesc;
struct TestSuiteDescEntryNameGetter;
struct LTTestSuiteLoaderImpl;

struct LTTestSuiteLoader : public TestSuiteLoader
{
   LTTestSuiteLoader();
   ~LTTestSuiteLoader();

	TestSuiteDesc* load(const std::string& path, TestSuiteDescEntryNameGetter* nameGetter);

   void unload();

private:
   LTTestSuiteLoaderImpl* This;
};

TESTNGPP_NS_END

#endif

