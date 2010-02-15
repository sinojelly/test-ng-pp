
#include <cxxtest/TestSuite.h>

#include <mockcpp/mockcpp.hpp>

#include <testngppst/ResourceCheckPoint.h>

#include <testngpp/runner/SimpleTestSuiteResultReporter.h>
#include <testngpp/listener/TestCaseResultReporter.h>
#include <testngpp/internal/TestSuiteInfoReader.h>
#include <testngpp/internal/TestCaseInfoReader.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class TestSimpleTestSuiteResultReporter: public CxxTest::TestSuite
{
private:

   TESTNGPPST_RCP checkpoint;

public:

   void setUp()
   {
      checkpoint = TESTNGPPST_SET_RESOURCE_CHECK_POINT();
   }
   void tearDown()
   {
      TESTNGPPST_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToReportNumberOfCrashedCases()
   {
      MockObject<TestCaseResultReporter> caseReporter;
      MockObject<TestCaseInfoReader> caseInfoReader;
      MockObject<TestSuiteInfoReader> suiteInfoReader;

      MOCK_METHOD(caseReporter, getTestCaseResult)
                  .stubs()
                  .will(returnValue((unsigned int)testngpp::TestCaseResultReporter::TR_CRASHED));

      SimpleTestSuiteResultReporter suiteReporter(caseReporter);

      suiteReporter.startTestSuite(suiteInfoReader);

      suiteReporter.startTestCase(caseInfoReader);
      suiteReporter.addCaseCrash(caseInfoReader);
      suiteReporter.endTestCase(caseInfoReader);

      suiteReporter.startTestCase(caseInfoReader);
      suiteReporter.addCaseCrash(caseInfoReader);
      suiteReporter.endTestCase(caseInfoReader);

      suiteReporter.startTestCase(caseInfoReader);
      suiteReporter.addCaseCrash(caseInfoReader);
      suiteReporter.endTestCase(caseInfoReader);

      suiteReporter.endTestSuite(suiteInfoReader);

      TS_ASSERT_EQUALS(3, suiteReporter.getNumberOfCrashedTestCases(suiteInfoReader));
   }
};
