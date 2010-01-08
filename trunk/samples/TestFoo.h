
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <iostream>

#include <testcpp/testcpp.hpp>

USING_TESTCPP_NS

class TestFoo : public TestFixture
{
public:
    void setUp()
    {}

    void tearDown()
    {}

    // Success

    /* this ml-comments is for test
 as well */ // same here

    /* this msl-comments is only for test */ //it should not hurt

    // @test
    void case0()
    {
       __BEGIN_TEST

       TS_ASSERT(false);

       std::cout << "running" << std::endl;

       __CLEANUP

       std::cout << "case 0 cleanup" << std::endl;

       __END_TEST
    }
			
    // @test
    // @timeout=15s
    void case1()
    {
       __BEGIN_TEST 

       TS_ASSERT(true);

       std::cout << "case 1 running" << std::endl;

       __CLEANUP

       std::cout << "case 1 cleanup" << std::endl;

       __END_TEST
    }

    // Fail
    // @test
    void case2()
    {
       ::exit(1);
    }

    // Success
    void testCase3() {}

    // Fail
    void testCase4()
    {
      TS_ASSERT_EQUALS(1, 2);
    }

    // Fail
    void testCase5()
    {
      TS_ASSERT(std::string("abc") == std::string("abcd"));
    }

    // Fail
    void testCase6()
    {
      TS_FAIL("not <implemented/> <yet>.");
    }

    // Success
    void testCase7()
    {
      TS_ASSERT_THROWS_NOTHING(testCase3());
    }

    // Success
    void testCase8()
    {
      TS_ASSERT_THROWS_ANYTHING(testCase6());
    }

    // Fail
    void testCase9()
    {
      TS_ASSERT_THROWS_ANYTHING(testCase3());
    }

    // Fail
    void testCase10()
    {
      TS_ASSERT_THROWS_NOTHING(testCase6());
    }

    // Fail
    void testCase11()
    {
      TS_ASSERT_THROWS(testCase3(), std::exception);
    }

#if 0
    void testCase12()
    {
      TS_ASSERT_THROWS(testCase3(), std::exception);
    }
#endif

    void testCase13()
    {
      TS_ASSERT_THROWS(testCase3(), std::exception);
    }

    void testCase14()
    {
      TS_ASSERT_THROWS(testCase3(), std::exception);
    }
};
