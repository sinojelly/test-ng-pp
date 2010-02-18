
#include <cxxtest/TestSuite.h> 
#include <testngpp/ResourceCheckPoint.h>

#include <testngpp/internal/AssertionFailure.h>

using namespace testngpp;

class TestAssertionFailure: public CxxTest::TestSuite
{
private:
   TESTNGPP_RCP checkpoint;

   TESTNGPP_NS::AssertionFailure* failure;
   
public:
   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();
      failure = new AssertionFailure("TestNothing.h", 23, "AssertionFailed");
   }

   void tearDown()
   {
      delete failure;
      TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
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
