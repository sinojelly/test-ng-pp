#include <testngppst/testngppst.hpp>
//#include <mem_checker/interface_4user.h>


FIXTURE(TestMemChecker)
{
    TEST(no memory leak in empty test)
    {
    }

	TEST(can detect memory leak caused by new operator)
	{
		char *p = new char[10];
	}

    TEST(no memory leaked after delete)
    {
        char *p = new char[55];
        //delete [] p;
    }
};