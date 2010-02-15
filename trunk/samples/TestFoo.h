
#include <cstdlib>
#include <string>
#include <iostream>

#include <testngpp/testngpp.hpp>

USING_TESTNGPP_NS

struct TestFixture12 {};

class TestFoo : public TestFixture
{
public:

    // @test
    void case0()
    {
       INFO("cleanup test");

       __TESTNGPP_DO

       WARN("gonna have an assertion failure");
       TS_ASSERT(false);

       __TESTNGPP_CLEANUP

       TS_ASSERT(true);

       __TESTNGPP_DONE
    }
			
    // @test
    // @timeout=15s
    void case1()
    {
       __TESTNGPP_DO

       TS_ASSERT(true);

       __TESTNGPP_CLEANUP

       TS_ASSERT(false);

       __TESTNGPP_DONE
    }

#if !defined(TESTNGPP_DISABLE_SANDBOX) || !TESTNGPP_DISABLE_SANDBOX
    // Fail
    // @test
    void case2()
    {
       ::exit(1);
    }
#endif

    // @test
    void case3() {}

    // @test
    void case4()
    {
      ASSERT_EQ(2, 2);
    }

    // @test
    void case5()
    {
      ASSERT_EQ(std::string("abc"), "abc");
    }

    // @test
    void case6()
    {
      FAIL("not implemented yet.");
    }

    // @test
    void case7()
    {
      unsigned int a = 0xabcdef01;
      unsigned int b = 0xabcdef02;
      ASSERT_SAME_DATA(&a, &b, sizeof(unsigned int));
    }

    // @test
    void case8()
    {
      unsigned int a = 0xabcdef01;
      unsigned int b = 0xabcdef01;
      ASSERT_SAME_DATA(&a, &b, sizeof(unsigned int));
    }

    // @test
    void case9()
    {
       ASSERT_DELTA(10, 9.9, 0.01);
    }

    // @test
    void case10()
    {
       ASSERT_DELTA(10, 9.9, 0.1);
    }

    void testCase11()
    {
      ASSERT_THROWS_NOTHING(case3());
    }

#if 0
    void testCase12()
    {
      TS_ASSERT_THROWS(testCase3(), std::exception);
    }
#endif

    // @test
    void case13()
    {
      ASSERT_THROWS(case3(), std::exception);
    }

    // @test
    void case14()
    {
      ASSERT_THROWS(case3(), std::exception);
    }
};


FIXTURE(ChineseFixture, 中文套件样例''~!@#$*%^&/\><;"各种各样的字符都应改允许")
{
    TEST(测试用例1: 1+1=2)
    {
       TS_ASSERT_EQUALS(2, 1+1);
    }

    TEST(测试用例2=>identical strings should equals each other)
    {
      TS_ASSERT(std::string("abc") == std::string("abcd"));
    }

    TEST(测试用例3: "两个相同的整数应该相等")
    {
      int a = 1;
      int b = 2;

      TS_ASSERT_EQUALS(a, b);
    }
};
