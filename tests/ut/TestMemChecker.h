#include <testngppst/testngppst.hpp>

// without this header, can only check memory leak caused by new, and can not specify the allocate file/line.
// with this header included, can check memory leak caused by new or malloc, and can specify the allocate file/line.
// maybe test file do not need this file, but including it in files to be tested is very important, especially for c files.
#include <mem_checker/interface_4user.h>

struct Dummy 
{
    Dummy(){}
};


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

    TEST(should not report memory leak when using placement new)
    {
        char a[10];
        Dummy *p = new(a) Dummy(); // not report memory leak, only show info message :warning: debug_new used with placement new.
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
        STOP_MEM_CHECKER(); //stopMemChecker();
        char *p = new char[3]; // should not report memory leak
    }

    TEST(user can reopen memory checker in a testcase)
    {
        STOP_MEM_CHECKER(); //stopMemChecker();
        char *p = new char[4]; // should not report memory leak
        OPEN_MEM_CHECKER();
        p = new char[5];  // should report memory leak
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

