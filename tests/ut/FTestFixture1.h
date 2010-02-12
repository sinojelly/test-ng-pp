
#ifndef __FIXTURE_TEST_FIXTURE1_H__
#define __FIXTURE_TEST_FIXTURE1_H___

#include <testngpp/ResourceCheckPoint.h>

#include <testngpp/internal/TestCase.h>
#include <testngpp/TestFixture.h>
#include <testngpp/internal/TestFixtureDesc.h>
#include <testngpp/runner/SimpleTestCaseRunner.h>
#include <testngpp/runner/TestCaseResultCollector.h>

USING_MOCKCPP_NS
USING_TESTNGPP_NS

class FTestFixture1
{
   struct MyTestCase : public TestCase
   {
      MyTestCase(TestFixture* fixture
           , const std::string& nameOfCase
           , const std::string& nameOfFixture
           , const std::string& file
           , unsigned int line)
           : m_fixture(fixture)
           , TestCase(nameOfCase, nameOfFixture, 0, file, line)
      {}

      TestFixture* getFixture() const { return m_fixture; }
      void run(bool) {}
      void setUp(TestFixture*)
      {
         m_fixture->setUp();
      }
      void tearDown()
      {
         m_fixture->tearDown();
      }
     
      TestFixture* m_fixture;
   };

public:

   TestFixtureDesc* desc;
   TestCase* testcase[1];

   MockObject<TestFixture> fixture;
   MockObject<TestCaseResultCollector> collector;

public:

   void setUp()
   {
		testcase[0] = new MyTestCase(fixture, "testCase1", "TestNothing", "TestNothing.h", 11);
      desc = new TestFixtureDesc("TestNothing", "TestNothing.h", testcase, 1);
   }

   void tearDown()
   {
      delete testcase[0];
      delete desc;

      fixture.reset();
      collector.reset();
   }

   void verify()
   {
      fixture.verify();
      collector.verify();
   }
};

#endif

