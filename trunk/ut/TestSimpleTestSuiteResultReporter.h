
#include <cxxtest/TestSuite.h>

#include <mockcpp/mockcpp.hpp>

#include <testngpp/ResourceCheckPoint.h>

#include <testngpp/runner/SimpleTestSuiteResultReporter.h>
#include <testngpp/runner/TestCaseResultReporter.h>
#include <testngpp/internal/TestSuiteInfoReader.h>
#include <testngpp/internal/TestCaseInfoReader.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestSimpleTestSuiteResultReporter: public CxxTest::TestSuite
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

   void testShouldBeAbleToReportNumberOfCrashedCases()
   {
      MockObject<TestCaseResultReporter> caseReporter;
      MockObject<TestCaseInfoReader> caseInfoReader;
      MockObject<TestSuiteInfoReader> suiteInfoReader;

      caseReporter.METHOD(TestCaseResultReporter::getTestCaseResult)
                  .stubs()
                  .will(returnValue((unsigned int)testngpp::TestCaseResultReporter::CRASHED));

      SimpleTestSuiteResultReporter suiteReporter(caseReporter);

      suiteReporter.startTestSuite(suiteInfoReader);

      suiteReporter.startTestCase(caseInfoReader);
      suiteReporter.addCaseCrash(caseInfoReader);

      suiteReporter.startTestCase(caseInfoReader);
      suiteReporter.addCaseCrash(caseInfoReader);

      suiteReporter.startTestCase(caseInfoReader);
      suiteReporter.addCaseCrash(caseInfoReader);

      suiteReporter.endTestSuite(suiteInfoReader);

      TS_ASSERT_EQUALS(3, suiteReporter.getNumberOfCrashedTestCases(suiteInfoReader));
   }
};
