
#include <cstdlib>
#include <string>
#include <iostream>

#include <testngpp/testngpp.hpp>

USING_TESTNGPP_NS

struct Hell
{
   Hell() { p = new char[10]; }
   ~Hell() { delete [] p; }

   char *p;
};

Hell hell[100];

struct TestBar1 : public TESTNGPP_NS::TestFixture
{
    // @test(id=1, depends=4, tags="ft slow")
    void case10()
    {
       INFO("this is case 10");
       ASSERT_FALSE(true);
    }

    // @test(id=2, depends=1, tags="it fast")
    void case11()
    {
       INFO("this is case 11");
       ASSERT_TRUE(true);
    }

    // @test(id=3)
    TEST(case13)
    {
       INFO("this is case 13");
    }
			
    // @test(id=4, depends=3, tags="it slow")
    TEST(case14)
    {
       INFO("this is case 14");
       ASSERT_TRUE(true);
    }

    // @test(depends=4, tags="ut fast")
    TEST(case15)
    {
       INFO("this is case 15");
       ASSERT_FALSE(true);
    }

    // @test(depends=2, tags="ft slow empty")
    TEST(case16)
    {
       INFO("this is case 16");
	    exit(-1);
	 }
};

class TestBar2 : public TestFixture
{
public:
    // @test
    void case20()
    {
      WARN("this test is gonna crash 'coz of invalid memory access");
	   char* p = 0;
	   (*p) = 0;
      TS_ASSERT(true);
    }
};

// @fixture(tags=succ)
struct TestBar3 : public TESTNGPP_NS::TestFixture
{
   // @test
   void case30()
   {}

   // @test
   void case31()
   {}

   // @test
   void case32()
   {}

   // @test
   void case33()
   {}

   // @test
   void case34()
   {}

   // @test
   void case35()
   {}

   // @test
   void case36()
   {}

   // @test
   void case37()
   {}

   // @test
   void case38()
   {}

   // @test
   void case39()
   {}
};
