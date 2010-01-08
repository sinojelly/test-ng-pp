

#include <cxxtest/TestSuite.h>

#include <mockcpp/mockcpp.hpp>

#include <testcpp/ResourceCheckPoint.h>

#include <testcpp/utils/StringToOptions.h>

#include <iostream>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestStringToOptions: public CxxTest::TestSuite
{
private:

   TESTCPP_RCP checkpoint;

public:

   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();
   }
   void tearDown()
   {
      try {
      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
      }
      catch(TESTCPP_NS::AssertionFailure& e)
      {
         std::cerr << e.what() << std::endl;
      }
      catch(TESTCPP_NS::Error& e)
      {
         std::cerr << e.what() << std::endl;
      }
   }

   void testShouldBeAbleToGetNumberOfArgsOfSimpleString()
   {
     TS_ASSERT_EQUALS(3, StringToOptions::getNumberOfArgsInString("abc bc c"));
   }

   void testShouldBeAbleToGetNumberOfArgsOfSimpleStringWithLeadingBlank()
   {
     TS_ASSERT_EQUALS(3, StringToOptions::getNumberOfArgsInString(" a bcd c"));
   }

   void testShouldBeAbleToGetNumberOfArgsOfSimpleStringWithTrailingBlank()
   {
     TS_ASSERT_EQUALS(3, StringToOptions::getNumberOfArgsInString("abc bcd cdef "));
   }

   void testShouldBeAbleToGetNumberOfArgsOfSimpleStringWithLeadingAndTrailingBlank()
   {
     TS_ASSERT_EQUALS(3, StringToOptions::getNumberOfArgsInString(" abc bcd c "));
   }

   void testShouldBeAbleToGetNumberOfArgsOfSimpleStringWithTab()
   {
     TS_ASSERT_EQUALS(3, StringToOptions::getNumberOfArgsInString("	 abc		bcd c	 	"));
   }

   void testShouldBeAbleToGetNumberOfArgsOfSimpleStringWithDoubleQuoteOptions()
   {
     TS_ASSERT_EQUALS(2, StringToOptions::getNumberOfArgsInString("abc \"bcd cdef\" "));
   }

   void testShouldBeAbleToGetNumberOfArgsOfSimpleStringWithDoubleQuoteInsideOptions()
   {
     TS_ASSERT_EQUALS(3, StringToOptions::getNumberOfArgsInString(" ./abc -l\" bcd -lcdef -s \" -s "));
   }

   void testShouldBeAbleToParseToCArgs()
   {
      StringToOptions::CArgs cargs = StringToOptions::parse(" ./abc -l\" bcd -lcdef -s \" -s");

      TS_ASSERT_EQUALS(3, cargs.first);

      TS_ASSERT(0 != cargs.second[0]);
      TS_ASSERT(0 != cargs.second[1]);
      TS_ASSERT(0 != cargs.second[2]);

      TS_ASSERT_EQUALS(std::string("./abc"), std::string(cargs.second[0]));
      TS_ASSERT_EQUALS(std::string("-l\" bcd -lcdef -s \""), std::string(cargs.second[1]));
      TS_ASSERT_EQUALS(std::string("-s"), std::string(cargs.second[2]));

      delete [] cargs.second[0];
      delete [] cargs.second[1];
      delete [] cargs.second[2];
      delete [] cargs.second;
   }
};
