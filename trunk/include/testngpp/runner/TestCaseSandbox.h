
#ifndef __TESTNGPP_TESTCASE_SANDBOX_H
#define __TESTNGPP_TESTCASE_SANDBOX_H

#include <testngpp/testngpp.h>

#include <testngpp/Error.h>

#include <testngpp/runner/SandboxTypedef.h>
#include <testngpp/runner/EOFError.h>

TESTNGPP_NS_START

struct TestCaseInfoReader;
struct EnvironmentCleaner;
struct TestCase;
struct TestFixtureDesc;
struct TestCaseRunner;
struct TestCaseResultCollector;
struct TestCaseSandboxImpl;

struct TestCaseSandbox
{
   SandboxId getSandboxId() const;
   ChannelId getChannelId() const;

   void handle() throw (Error, EOFError);

   bool isDead() const;

   void cleanup();

   ~TestCaseSandbox();

	static TestCaseSandbox*
	createInstance(EnvironmentCleaner* cleaner, \
          TestCase* testcase, \
          TestFixtureDesc* fixture, \
          TestCaseRunner* runner,
          TestCaseResultCollector* collector);

private:
	TestCaseSandbox();

private:
   TestCaseSandboxImpl* This;
};

TESTNGPP_NS_END

#endif
