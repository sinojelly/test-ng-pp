
#include <cstdlib>
#include <string>
#include <iostream>

#include <testcpp/testcpp.hpp>

USING_TESTCPP_NS

class TestBar : public testcpp::TestFixture
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
       TS_ASSERT(false);
    }
			
    // @test
    // @timeout=15s
    void case1()
    {
       TS_ASSERT(true);
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

struct TestBar1 : public TESTCPP_NS::TestFixture
{
    // @test
    void case0()
    {
       TS_ASSERT(true);
    }
			
};

class TestBar2 : public TestFixture
{
public:
    // @test
    void case0()
    {
       TS_ASSERT(true);
    }
			
};
