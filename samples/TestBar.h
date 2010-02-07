
#include <cstdlib>
#include <string>
#include <iostream>

#include <testcpp/testcpp.hpp>

USING_TESTCPP_NS

// @ fixture(tags="bar")
struct TestBar1 : public TESTCPP_NS::TestFixture
{
    // @test(id=1, depends=4, tags="ft slow")
    void case10()
    {
       TS_ASSERT(false);
    }

    // @test(id=2, depends=1, tags="it fast")
    void case11()
    {
       TS_ASSERT(true);
    }

    // @test(depends=2)
    TEST(case13)
    {
    }
			
    // @test(id=4, tags="it slow")
    TEST(case14)
    {
       TS_ASSERT(true);
    }

    // @test(depends=4, tags="ut fast")
    TEST(case15)
    {
       TS_ASSERT(true);
    }

    // @test(depends=2, tags="ft slow empty")
    TEST(case16)
    {}
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
