

#include <cxxtest/TestSuite.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/ResourceCheckPoint.h>

#include <testngpp/runner/TagsFilters.h>
#include <testngpp/runner/TagsParser.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestTagsParser: public CxxTest::TestSuite
{
private:

   TESTNGPP_RCP checkpoint;

public:

   void setUp()
   {
      checkpoint = TESTNGPP_SET_RESOURCE_CHECK_POINT();
   }
   void tearDown()
   {
      TESTNGPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToParseSingleGroup()
   {
      std::string tagsSpec("tag1 tag2 tag3");
      
      TagsFilters* filter = TagsParser::parse(tagsSpec);
      
      TS_ASSERT(0 != filter);
      
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(!filter->startOnNext());
      
      delete filter;
   }
   
   void testShouldBeAbleToParseMultiGroups()
   {
      std::string tagsSpec("tag1 tag2>tag3 tag4>tag5");
      
      TagsFilters* filter = TagsParser::parse(tagsSpec);
      
      TS_ASSERT(0 != filter);
      
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      
      TS_ASSERT(!filter->startOnNext());
      
      delete filter;  
   }
   
   void testShouldBeAbleToRecognizeMatchAll1()
   {
      std::string tagsSpec("*>tag1 tag2>tag5");
      
      TagsFilters* filter = TagsParser::parse(tagsSpec);
      
      TS_ASSERT(0 != filter);
      
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      
      TS_ASSERT(!filter->startOnNext());
      
      delete filter;  
   }
   
   void testShouldBeAbleToRecognizeMatchAll2()
   {
      std::string tagsSpec("tag1 tag2>*>tag5");
      
      TagsFilters* filter = TagsParser::parse(tagsSpec);
      
      TS_ASSERT(0 != filter);
      
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      
      TS_ASSERT(!filter->startOnNext());
      
      delete filter;  
   }
   
   void testShouldBeAbleToRecognizeMatchAll3()
   {
      std::string tagsSpec("tag1 tag2>tag3 tag4>*");
      
      TagsFilters* filter = TagsParser::parse(tagsSpec);
      
      TS_ASSERT(0 != filter);
      
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      
      TS_ASSERT(!filter->startOnNext());
      
      delete filter;  
   }
   
   void testShouldBeAbleToRecognizeMatchAllNotInTopScope()
   {
      std::string tagsSpec("tag1 tag2>tag3 tag4>(*)");
      
      TS_ASSERT_THROWS(TagsParser::parse(tagsSpec), TESTNGPP_NS::Error);
   }

   void testShouldBeAbleToRecognizeSignOfNoTag()
   {
      std::string tagsSpec("tag1 $ >tag3 tag4>*");
      
      TagsFilters* filter = TagsParser::parse(tagsSpec);
      
      TS_ASSERT(0 != filter);
      
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      TS_ASSERT(filter->startOnNext());
      
      TS_ASSERT(!filter->startOnNext());
      
      delete filter;  
   }
   
   void testDollarSignShouldNotBeUsedInAndGroup()
   {
      std::string tagsSpec("[tag1 $] >tag3 tag4>*");
      
      TS_ASSERT_THROWS(TagsParser::parse(tagsSpec), TESTNGPP_NS::Error);
   }
};
