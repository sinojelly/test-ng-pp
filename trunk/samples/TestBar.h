
#include <cstdlib>
#include <string>
#include <iostream>

#include <testcpp/testcpp.hpp>

USING_TESTCPP_NS

struct TestBar1 : public TESTCPP_NS::TestFixture
{
    // @test(id=1, depends=4)
    void case10()
    {
       TS_ASSERT(false);
    }

    // @test(id=2, depends=1)
    void case11()
    {
       TS_ASSERT(true);
    }

    // @test(depends=2)
    TEST(case13)
    {}
			

    // @test(id=4)
    TEST(case14)
    {
       TS_ASSERT(true);
    }

    // @test(depends=4)
    TEST(case15)
    {
       TS_ASSERT(true);
    }
};

class TestBar2 : public TestFixture
{
public:
    // @test
    void case20()
    {
       TS_ASSERT(true);
    }
			
};
