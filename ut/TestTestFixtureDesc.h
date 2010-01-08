
#include <cxxtest/TestSuite.h> 
#include <testcpp/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testcpp/internal/TestFixtureDesc.h>
#include <testcpp/internal/TestCase.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestTestFixtureDesc: public CxxTest::TestSuite
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

   TestFixtureDesc* desc;
   
   TestCase* testCases[2];

   TestFixture fixture;
public:
   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();

      testCases[0] = new MyTestCase("testCase0", "TestNothing", "TestNothing.h", 1);
      testCases[1] = new MyTestCase("testCase1", "TestNothing", "TestNothing.h", 11);

      desc = new TestFixtureDesc("TestNothing" , "TestNothing.h" , &fixture, testCases , 2);
   }
   void tearDown()
   {
      delete desc;
      delete testCases[0];
      delete testCases[1];
  
      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToGetFileNameOfFixture()
   {
      TS_ASSERT_EQUALS(std::string("TestNothing.h"), desc->getFileName());
   }

   void testShouldBeAbleToGetFixtureName()
   {
      TS_ASSERT_EQUALS(std::string("TestNothing"), desc->getName());
   }

   void testShouldBeAbleToGetNumberOfTestCases()
   {
      TS_ASSERT_EQUALS((unsigned int)2, desc->getNumberOfTestCases());
   }

   void testShouldBeAbleToGetTestCaseByIndex()
   {
      TS_ASSERT_EQUALS(testCases[0], desc->getTestCase(0));
      TS_ASSERT_EQUALS(testCases[1], desc->getTestCase(1));
   }

   void testShouldGetNullPointerIfTryingToGetTestCaseWithIndexWhichExceedsTheNumberOfTestCases()
   {
      TS_ASSERT_EQUALS((TestCase*)0, desc->getTestCase(2));
   }

   void testShouldImplementTestFixtureInfoReader()
   {
      TS_ASSERT(dynamic_cast<TestFixtureInfoReader*>(desc) != 0);
   }
};
