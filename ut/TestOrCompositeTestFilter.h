
#include <cxxtest/TestSuite.h>

#include <mockcpp.hpp>

#include <testcpp/ResourceCheckPoint.h>

#include <OrCompositeTestFilter.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestOrCompositeTestFilter: public CxxTest::TestSuite
{
private:

   TESTCPP_RCP checkpoint;

   MockObject<TestFixtureInfoReader> fixture;
   MockObject<TestCaseInfoReader> testcase;

   MockObject<TestFilter> filter1;
   MockObject<TestFilter> filter2;
   MockObject<TestFilter> filter3;

   OrCompositeTestFilter* filter;
public:

   void setUp()
   {
      checkpoint = TESTCPP_SET_RESOURCE_CHECK_POINT();

      filter1.METHOD(TestFilter::isFixtureMatch)
         .defaults().will(returnValue(false));

      filter1.METHOD(TestFilter::isCaseMatch)
         .defaults().will(returnValue(false));

      filter2.METHOD(TestFilter::isFixtureMatch)
         .defaults().will(returnValue(false));

      filter2.METHOD(TestFilter::isCaseMatch)
         .defaults().will(returnValue(false));

      filter3.METHOD(TestFilter::isFixtureMatch)
         .defaults().will(returnValue(false));

      filter3.METHOD(TestFilter::isCaseMatch)
         .defaults().will(returnValue(false));

      filter = new OrCompositeTestFilter();

      filter->addFilter(filter1);
      filter->addFilter(filter2);
      filter->addFilter(filter3);
   }

   void tearDown()
   {
      delete filter;

      filter1.reset();
      filter2.reset();
      filter3.reset();

      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldMatchesAFixutreIfAnyOfAddedFiltersMatchesIt()
   {
      filter2.METHOD(TestFilter::isFixtureMatch)
         .stubs()
         .with(eq((const TestFixtureInfoReader*)fixture))
         .will(returnValue(true));

      TS_ASSERT(filter->isFixtureMatch(fixture));
   }

   void testShouldNotMatchAFixtureIfAllAddedFiltersClaimsNotMatch()
   {
      TS_ASSERT(!filter->isFixtureMatch(fixture));
   }

   void testShouldMatchesATestcaseIfAnyOfAddedFiltersMatchesIt()
   {
      filter2.METHOD(TestFilter::isCaseMatch)
         .stubs()
         .with(eq((const TestCaseInfoReader*)testcase))
         .will(returnValue(true));

      TS_ASSERT(filter->isCaseMatch(testcase));
   }

   void testShouldNotMatchATestcaseIfAllAddedFiltersClaimsNotMatch()
   {
      TS_ASSERT(!filter->isCaseMatch(testcase));
   }

   void testShouldDeleteAllTestFiltersWhichAreAddedAsComposite()
   {
      filter1.willBeDeleted();
      filter2.willKeepAlive();
      filter3.willBeDeleted();

      OrCompositeTestFilter* thisFilter = new OrCompositeTestFilter();

      thisFilter->addFilter(filter1, true);
      thisFilter->addFilter(filter2);
      thisFilter->addFilter(filter3, true);

      delete thisFilter;

      filter1.verify();
      filter2.verify();
      filter3.verify();
   }
};
