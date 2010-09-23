#include <testngppst/testngppst.hpp>
#include <mem_checker/interface_4user.h>


FIXTURE(TestMemChecker)
{
    TEST(no memory leak in empty test)
    {
    }

	TEST(can detect memory leak caused by new operator)
	{
		char *p = new char[10]; // should report memory leak
	}

    TEST(no memory leaked after delete)
    {
        char *p = new char[55];
        delete [] p;
    }

    TEST(can detect memory leak caused by malloc)
    {
        char* p = (char *)malloc(33); // should report memory leak
    }

    TEST(no memory leaked after free)
    {
        char *p = (char *)malloc(13);
        free(p);
    }

    TEST(can stop memory checker in a testcase)
    {
        stopMemChecker();
        char *p = new char[3]; // should not report memory leak
    }

    TEST(support checking memory leak in c file when interface_4user.h included in the c file)
    {
        //char *p = (char*)mallocInCFile(7);   // should report memory leak, i have not found a way to implement this testcase yet, but i test in another project.
    }

    TEST(no memory leak report after free in c file when interface_4user.h included in the c file)
    {
        //char *p = (char*)mallocInCFile(7);   // should not report memory leak, i have not found a way to implement this testcase yet, but i test in another project.
        //freeInCFile(p);
    }
};