
#ifndef __TESTNGPP_TEST_SUITE_LOADER_H
#define __TESTNGPP_TEST_SUITE_LOADER_H

#include <string>

#include <testngpp/testngpp.h>

TESTNGPP_NS_START

struct TestSuiteDesc;
struct TestSuiteDescEntryNameGetter;

struct TestSuiteLoader
{
	virtual TestSuiteDesc* load(const std::string& path, TestSuiteDescEntryNameGetter* getter) = 0;

   virtual void unload() = 0;

	virtual ~TestSuiteLoader() {}
};

TESTNGPP_NS_END

#endif
