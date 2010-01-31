
#include <testngpp/Error.h>
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/internal/TestFixtureInfoReader.h>
#include <testngpp/internal/TestFixtureDesc.h>

#include <testngpp/runner/TestFixtureRunner.h>
#include <testngpp/runner/TestHierarchyRunner.h>
#include <testngpp/runner/TestCaseHierarchy.h>
#include <testngpp/runner/TestFixtureResultCollector.h>
#include <testngpp/runner/InternalError.h>
#include <testngpp/runner/TestCaseFilter.h>


TESTNGPP_NS_START

struct TestFixtureRunnerImpl
{
   TestFixtureRunnerImpl(TestHierarchyRunner* runner);
   ~TestFixtureRunnerImpl();

   void run( TestFixtureDesc* desc
           , TestFixtureResultCollector* collector
           , const TestCaseFilter* filter);

   TestHierarchyRunner* hierarchyRunner; // Y
};

/////////////////////////////////////////////////////////////////
TestFixtureRunnerImpl::
TestFixtureRunnerImpl(TestHierarchyRunner* runner)
	: hierarchyRunner(runner)
{
}

/////////////////////////////////////////////////////////////////
TestFixtureRunnerImpl::
~TestFixtureRunnerImpl()
{
   delete hierarchyRunner;
}

/////////////////////////////////////////////////////////////////
TestFixtureRunner::
TestFixtureRunner(TestHierarchyRunner* runner)
	: This(new TestFixtureRunnerImpl(runner))
{
}

/////////////////////////////////////////////////////////////////
TestFixtureRunner::~TestFixtureRunner()
{
   delete This;
}

/////////////////////////////////////////////////////////////////
void TestFixtureRunnerImpl::
run( TestFixtureDesc* desc
   , TestFixtureResultCollector* collector
   , const TestCaseFilter* filter)
{
   TestCaseHierarchy* hierarchy = new TestCaseHierarchy(desc, filter);

   collector->startTestFixture(desc);

   __TESTNGPP_TRY
   {
      hierarchyRunner->run(hierarchy, collector);
   }
   __TESTNGPP_CATCH(Error& e)
   {
      collector->addFixtureError(desc, e.what());
   }
   __TESTNGPP_CATCH_ALL
   {
      collector->addFixtureError(desc, "Unknown Error");
   }
   __TESTNGPP_END_TRY

   collector->endTestFixture(desc);

   delete hierarchy;
}


/////////////////////////////////////////////////////////////////
void TestFixtureRunner::
run( TestFixtureDesc* desc
   , TestFixtureResultCollector* collector
   , const TestCaseFilter* filter)
{
   return This->run(desc, collector, filter);
}

/////////////////////////////////////////////////////////////////

TESTNGPP_NS_END

