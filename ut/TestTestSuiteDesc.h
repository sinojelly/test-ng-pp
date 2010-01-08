
#include <cxxtest/TestSuite.h> 
#include <testcpp/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testcpp/internal/TestCase.h>
#include <testcpp/internal/TestFixtureDesc.h>
#include <testcpp/internal/TestSuiteDesc.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestTestSuiteDesc: public CxxTest::TestSuite
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
   TESTCPP_RCP checkpoint;

   TestFixtureDesc* desc[2];

   TestCase* testCases[2][2];

   TestFixture fixture;

public:

   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();

      testCases[0][0] = new MyTestCase("testCase0", "TestNothing0", "TestNothing0.h", 1);
      testCases[0][1] = new MyTestCase("testCase1", "TestNothing0", "TestNothing0.h", 11);

      testCases[1][0] = new MyTestCase("testCase0", "TestNothing1", "TestNothing1.h", 1);
      testCases[1][1] = new MyTestCase("testCase1", "TestNothing1", "TestNothing1.h", 11);

      desc[0] = new TestFixtureDesc("TestNothing0" , "TestNothing0.h" , &fixture, testCases[0] , 2);
      desc[1] = new TestFixtureDesc("TestNothing1" , "TestNothing1.h" , &fixture, testCases[1] , 2);

   }

   void tearDown()
   {
      delete desc[0];
      delete desc[1];
      delete testCases[0][0];
      delete testCases[0][1];
      delete testCases[1][0];
      delete testCases[1][1];

      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToGetNameOfSuite()
   {
      TestSuiteDesc suiteDesc("TestNothingSuite", desc, 2);

      TS_ASSERT_EQUALS(std::string("TestNothingSuite"), suiteDesc.getName());
   }

   void testShouldBeAbleSetFileName()
   {
      TestSuiteDesc suiteDesc("TestNothingSuite", desc, 2);

      TS_ASSERT_EQUALS(std::string(""), suiteDesc.getFileName());

      suiteDesc.setFileName("ut/TestNothingSuite.so");

      TS_ASSERT_EQUALS(std::string("ut/TestNothingSuite.so"), suiteDesc.getFileName());
   }

   void testShouldBeAbleToGetNumberOfTestCases()
   {
      TestSuiteDesc suiteDesc("TestNothingSuite", desc, 2);

      TS_ASSERT_EQUALS((unsigned int)4, suiteDesc.getNumberOfTestCases());
   }

   void testShouldBeAbleToGetNumberOfTestFixtures()
   {
      TestSuiteDesc suiteDesc("TestNothingSuite", desc, 2);

      TS_ASSERT_EQUALS((unsigned int)2, suiteDesc.getNumberOfTestFixtures());
   }

   void testShouldBeAbleToGetFixtureDescByIndex()
   {
      TestSuiteDesc suiteDesc("TestNothingSuite", desc, 2);

      TS_ASSERT_EQUALS(desc[0], suiteDesc.getTestFixture(0));
      TS_ASSERT_EQUALS(desc[1], suiteDesc.getTestFixture(1));
   }

   void testShouldGetNullPointerIfGivenIndexExceedsTheNumberOfFixturesInArray()
   {
      TestSuiteDesc suiteDesc("TestNothingSuite", desc, 2);

      TS_ASSERT_EQUALS((TestFixtureDesc*)0, suiteDesc.getTestFixture(2));
   }
};
