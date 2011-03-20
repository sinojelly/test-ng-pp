#include <testngppst/testngppst.hpp>
#include <mockcpp/mokc.h>


FIXTURE(TestAsserter)
{
    TEST(the difference of two double values less than FLT_EPSILON should be judged as equal)
    {
        ASSERT_DBL_EQ(0.01, 0.01);
        ASSERT_DBL_EQ(0.1, 0.1 + FLT_EPSILON * 0.1);
    }

    TEST(the difference of two double values larger than FLT_EPSILON should be judged as not equal)
    {
        ASSERT_DBL_NE(0.01, 1.01);
        ASSERT_DBL_NE(0.1, 0.1 + FLT_EPSILON * 2);
    }
	
	TEST(double 0.6 add float 0.1 should equal to double 0.7)
    {
		double d = 0.6;
		float  f = (float)0.1;

		ASSERT_DBL_EQ(d + f, double(0.7));	  // if use DBL_EPSILON in ASSERT macro, it print:  found ((double)0.7 != (double)0.7), it's very strange.
    }	

	TEST(float 0.6 add float 0.1 should equal to float 0.7)
    {
		float f1 = (float)0.6;
		float f2 = (float)0.1;

		ASSERT_DBL_EQ(f1 + f2, float(0.7));	  // if use ASSERT_EQ, it's fail.
    }		
};

