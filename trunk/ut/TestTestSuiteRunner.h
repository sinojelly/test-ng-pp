
#include <cxxtest/TestSuite.h> 
#include <testngpp/ResourceCheckPoint.h>

#include <mockcpp.hpp>

#include <TestFilter.h>
#include <TestCase.h>
#include <TestFixture.h>
#include <TestFixtureDesc.h>
#include <TestSuiteRunner.h>
#include <TestSuiteLoader.h>
#include <TestFixtureRunner.h>
#include <TestResultCollector.h>
#include <TestSuiteDesc.h>
#include <Error.h>

#include <iostream>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestTestSuiteRunner: public CxxTest::TestSuite
{
   struct MyTestCase : public TestCase
   {
      MyTestCase( const std::string& nameOfCase
           , const std::string& nameOfFixture
           , const std::string& file
           , unsigned int line)
           : TestCase(nameOfCase, nameOfFixture, file, line)
      {}

      void run() {}
      void setUp() {}
      void tearDown() {}
   };

private:
   TESTNGPP_RCP checkpoint;

   TestFixtureDesc* desc[2];

   TestCase* testCases[2][2];

   MockObject<TestSuiteLoader> suiteLoader;
   MockObject<TestFixtureRunner> fixtureRunner;
   MockObject<TestResultCollector> collector;
   MockObject<TestFilter> filter;

   TestSuiteDesc* suiteDesc;

   TestFixture fixture;

public:

   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();

      testCases[0][0] = new MyTestCase("testCase0", "TestNothing0", "TestNothing0.h", 1);
      testCases[0][1] = new MyTestCase("testCase1", "TestNothing0", "TestNothing0.h", 11);

      testCases[1][0] = new MyTestCase("testCase0", "TestNothing1", "TestNothing1.h", 1);
      testCases[1][1] = new MyTestCase("testCase1", "TestNothing1", "TestNothing1.h", 11);

      desc[0] = new TestFixtureDesc("TestNothing0" , "TestNothing0.h", testCases[0] , 2);
      desc[1] = new TestFixtureDesc("TestNothing1" , "TestNothing1.h", testCases[1] , 2);

		suiteDesc = new TestSuiteDesc("TestNothingSuite", desc, 2);

      std::string file("TestNothing");

      MOCK_METHOD(filter, isFixtureMatch)
           .defaults()
           .will(returnValue(true));

      MOCK_METHOD(suiteLoader, load)
           .defaults()
           .with(endWith(file))
           .will(returnValue(suiteDesc));

      MOCK_METHOD(suiteLoader, unload)
           .defaults();

      MOCK_METHOD(fixtureRunner, run)
           .defaults()
           .with(eq(desc[0]), eq((TestFixtureResultCollector*)collector));

      MOCK_METHOD(fixtureRunner, run)
           .defaults()
           .with(eq(desc[1]), eq((TestFixtureResultCollector*)collector));

      MOCK_METHOD(collector, startTestSuite)
           .defaults()
           .with(eq((TestSuiteInfoReader*)suiteDesc));

      MOCK_METHOD(collector, endTestSuite)
           .defaults()
           .with(eq((TestSuiteInfoReader*)suiteDesc));
   }

   void tearDown()
   {
      suiteLoader.reset();
      fixtureRunner.reset();
      collector.reset();
      filter.reset();

      delete suiteDesc;
      delete desc[0];
      delete desc[1];
      delete testCases[0][0];
      delete testCases[0][1];
      delete testCases[1][0];
      delete testCases[1][1];

      TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void verify()
   {
      suiteLoader.verify();
      fixtureRunner.verify();
      collector.verify();
   }


   void testShouldBeAbleToRunnAllTestFixtures()
   {

      std::string file("TestNothing");

      MOCK_METHOD(suiteLoader, load)
           .expects(once())
           .with(endWith(file))
           .will(returnValue(suiteDesc));

      MOCK_METHOD(suiteLoader, unload)
           .expects(once());

      MOCK_METHOD(fixtureRunner, run)
           .expects(once())
           .with(eq(desc[0]), eq((TestFixtureResultCollector*)collector));

      MOCK_METHOD(fixtureRunner, run)
           .expects(once())
           .with(eq(desc[1]), eq((TestFixtureResultCollector*)collector));

      MOCK_METHOD(collector, startTestSuite)
           .expects(once())
           .with(eq((TestSuiteInfoReader*)suiteDesc))
           .id("start");

      MOCK_METHOD(collector, endTestSuite)
           .expects(once())
           .with(eq((TestSuiteInfoReader*)suiteDesc))
           .after("start");

      ////////////////////////////////////////////////////
      TestSuiteRunner runner(suiteLoader, fixtureRunner);

      runner.run(file, collector, filter);

      ////////////////////////////////////////////////////
      verify();
   }

   void testShouldReportToCollectorIfLoadTestSuiteFailed()
   {
      std::string file("TestNothing");

      MOCK_METHOD(suiteLoader, load)
           .expects(once())
           .with(endWith(file))
           .will(throws(Error("File Not Found")));

      MOCK_METHOD(collector, addError)
           .expects(once())
           .with(contains("File Not Found"));

      MOCK_METHOD(fixtureRunner, run)
           .expects(never());

      ////////////////////////////////////////////////////
      TestSuiteRunner runner(suiteLoader, fixtureRunner);

      runner.run(file, collector, filter);

      ////////////////////////////////////////////////////
      verify();
   }
};
