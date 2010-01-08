
#include <cxxtest/TestSuite.h> 
#include <testcpp/ResourceCheckPoint.h>

#include <mockcpp/mockcpp.hpp>

#include <testcpp/AssertionFailure.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestAssertionFailure: public CxxTest::TestSuite
{
private:
   TESTCPP_RCP checkpoint;

   AssertionFailure* failure;
   
public:
   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();
      failure = new AssertionFailure("TestNothing.h", 23, "AssertionFailed");
   }

   void tearDown()
   {
      delete failure;
      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToGetFileName()
   {
      TS_ASSERT_EQUALS(std::string("TestNothing.h"), failure->getFileName());
   }

   void testShouldBeAbleToGetLineOfFile()
   {
      TS_ASSERT_EQUALS((unsigned int)23, failure->getLineOfFile());
   }

   void testShouldBeAbleToGetTheReasonOfFailure()
   {
      TS_ASSERT_EQUALS(std::string("AssertionFailed"), failure->what());
   }
};
