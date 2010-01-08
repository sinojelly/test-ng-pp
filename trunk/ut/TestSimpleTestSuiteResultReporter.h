
#include <cxxtest/TestSuite.h>

#include <testcpp/runner/SimpleTestSuiteResultReporter.h>
#include <testcpp/runner/TestCaseResultReporter.h>
#include <testcpp/internal/TestSuiteInfoReader.h>

USING_MOCKCPP_NS
USING_TESTCPP_NS

class TestSimpleTestSuiteResultReporter: public CxxTest::TestSuite
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
      TESTCPP_VERIFY_RESOURCE_CHECK_POINT(checkpoint);
   }

   void testShouldBeAbleToReportNumberOfCrashedCases()
   {
      MockObject<TestCaseResultReporter> caseReporter;
      MockObject<TestCaseInfoReader> caseInfoReader;
      MockObject<TestSuiteInfoReader> suiteInfoReader;

      caseReporter.METHOD(TestCaseResultReporter::getTestCaseResult)
                  .stubs()
                  .will(returnValue((unsigned int)testcpp::TestCaseResultReporter::CRASHED));

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
