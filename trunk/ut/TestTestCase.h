
#include <cxxtest/TestSuite.h> 
#include <testcpp/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testcpp/internal/TestCase.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestTestCase: public CxxTest::TestSuite
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

   TestCase * testCase;


public:
   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();
	   testCase = new MyTestCase("testShouldDoNothing", "TestNothing", "TestNothing.h", 23);
   }
   void tearDown()
   {
      delete testCase;
      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToGetFileName()
   {
      TS_ASSERT_EQUALS(std::string("TestNothing.h"), testCase->getFileName());
   }

   void testShouldBeAbleToGetTestCaseName()
   {
      TS_ASSERT_EQUALS(std::string("testShouldDoNothing"), testCase->getName());
   }

   void testShouldBeAbleToGetTestFixtureName()
   {
      TS_ASSERT_EQUALS(std::string("TestNothing"), testCase->getNameOfFixture());
   }

   void testShouldBeAbleToGetLineOfFile()
   {
      TS_ASSERT_EQUALS((unsigned int)23, testCase->getLineOfFile());
   }

   void testShouldImplementTestCaseInfoReader()
   {
      TS_ASSERT(dynamic_cast<TestCaseInfoReader*>(testCase) != 0);
   }
};
