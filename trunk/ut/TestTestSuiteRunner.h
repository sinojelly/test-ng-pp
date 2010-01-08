
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

      desc[0] = new TestFixtureDesc("TestNothing0" , "TestNothing0.h", &fixture, testCases[0] , 2);
      desc[1] = new TestFixtureDesc("TestNothing1" , "TestNothing1.h", &fixture, testCases[1] , 2);

		suiteDesc = new TestSuiteDesc("TestNothingSuite", desc, 2);

      std::string file("TestNothing");

      filter.METHOD(TestFilter::isFixtureMatch).defaults().will(returnValue(true));
      suiteLoader.METHOD(TestSuiteLoader::load)
                 .defaults()
                 .with(endWith(file))
                 .will(returnValue(suiteDesc));

      suiteLoader.METHOD(TestSuiteLoader::unload)
                 .defaults();

      fixtureRunner.METHOD(TestFixtureRunner::run)
                 .defaults()
                 .with(eq(desc[0]), eq((TestFixtureResultCollector*)collector));

      fixtureRunner.METHOD(TestFixtureRunner::run)
                 .defaults()
                 .with(eq(desc[1]), eq((TestFixtureResultCollector*)collector));

      collector.METHOD(TestSuiteResultCollector::startTestSuite)
               .defaults()
               .with(eq((TestSuiteInfoReader*)suiteDesc));

      collector.METHOD(TestSuiteResultCollector::endTestSuite)
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

      try{
        TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
      }
      catch(Exception& e)
      {
         std::cerr << e.what() << std::endl;
         throw;
      }
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

      suiteLoader.METHOD(TestSuiteLoader::load)
                 .expects(once())
                 .with(endWith(file))
                 .will(returnValue(suiteDesc));

      suiteLoader.METHOD(TestSuiteLoader::unload)
                 .expects(once());

      fixtureRunner.METHOD(TestFixtureRunner::run)
                 .expects(once())
                 .with(eq(desc[0]), eq((TestFixtureResultCollector*)collector));

      fixtureRunner.METHOD(TestFixtureRunner::run)
                 .expects(once())
                 .with(eq(desc[1]), eq((TestFixtureResultCollector*)collector));

      collector.METHOD(TestSuiteResultCollector::startTestSuite)
               .expects(once())
               .with(eq((TestSuiteInfoReader*)suiteDesc))
               .id("start");

      collector.METHOD(TestSuiteResultCollector::endTestSuite)
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

      suiteLoader.METHOD(TestSuiteLoader::load)
                 .expects(once())
                 .with(endWith(file))
                 .will(throws(Error("File Not Found")));

      collector.METHOD(TestResultCollector::addError)
               .expects(once())
               .with(contains("File Not Found"));

      fixtureRunner.METHOD(TestFixtureRunner::run)
                 .expects(never());

      ////////////////////////////////////////////////////
      TestSuiteRunner runner(suiteLoader, fixtureRunner);

      runner.run(file, collector, filter);

      ////////////////////////////////////////////////////
      verify();
   }
};
