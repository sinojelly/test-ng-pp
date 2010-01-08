
#include <TestFixtureDesc.h>
#include <TestCase.h>
#include <TestFixture.h>
#include <string>

#include <Exception.h>
#include <AssertionFailure.h>

USING_TESTCPP_NS
USING_MOCKCPP_NS

class MyTestFixture : public TestFixture
{
public:
   void setUp()
   {
   }

   void tearDown()
   {
   }

   void testCase1()
   {
      throw AssertionFailure("MyTestFixture.h", 12, "a != b");
   }

   void testCase2()
   {
      throw Exception(15, "MyTestFixture.h", "Exception");
   }

   void testCase3()
   {
   }
};

static MyTestFixture myTestFixture;

struct MyTestCase : public TestCase
{
   MyTestCase(const std::string name, unsigned int line)
	  : TestCase(name, "MyTestFixture", "MyTestFixture.h", line)
   {}

};

static struct MyTestCase1 : public MyTestCase
{
   MyTestCase1() : MyTestCase("testCase1", 5) {}

   void run()
   {
      myTestFixture.testCase1();
   }
}
myTestCase1;

static struct MyTestCase2 : public MyTestCase
{
   MyTestCase2() : MyTestCase("testCase1", 5) {}

   void run()
   {
      myTestFixture.testCase2();
   }
}
myTestCase2;

static struct MyTestCase3 : public MyTestCase
{
   MyTestCase3() : MyTestCase("testCase1", 5) {}

   void run()
   {
      myTestFixture.testCase3();
   }
}
myTestCase3;

static TestCase* myTestCases[] = { &myTestCase1, &myTestCase2, &myTestCase3 };

static TestFixtureDesc myTestFixtureDesc("MyTestFixture", "MyTestFixture.h", &myTestFixture, myTestCases, 3);

