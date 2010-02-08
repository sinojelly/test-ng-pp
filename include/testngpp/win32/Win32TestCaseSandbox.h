
#ifndef __TESTNGPP_WIN32_TESTCASE_SANDBOX_H
#define __TESTNGPP_WIN32_TESTCASE_SANDBOX_H

#include <windows.h>

#include <testngpp/testngpp.h>

#include <testngpp/Error.h>

#include <testngpp/runner/SandboxTypedef.h>
#include <testngpp/runner/EOFError.h>

TESTNGPP_NS_START

struct TestCaseInfoReader;
struct TestCase;
struct TestCaseRunner;
struct TestCaseResultCollector;
struct Win32TestCaseSandboxImpl;

struct Win32TestCaseSandbox
{
   HANDLE getSandboxId() const;
   HANDLE getChannelId() const;

   void handle() TESTNGPP_THROW(Error, EOFError);
   
   const TestCase* getTestCase() const;

   bool hasSucceeded() const;

   bool isDead() const;

   void cleanup();

   ~Win32TestCaseSandbox();

public:

	static Win32TestCaseSandbox*
	createInstance
         ( const TestCase* testcase
         , TestCaseRunner* runner
         , TestCaseResultCollector* collector
         , bool shouldReport);

private:
	Win32TestCaseSandbox();

private:
   Win32TestCaseSandboxImpl* This;
};

TESTNGPP_NS_END

#endif
