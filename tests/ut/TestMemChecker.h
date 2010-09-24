#include <testngppst/testngppst.hpp>
#include <mockcpp/mokc.h>

// without this header, can only check memory leak caused by new, and can not specify the allocate file/line.
// with this header included, can check memory leak caused by new or malloc, and can specify the allocate file/line.
// maybe test file do not need this file, but including it in files to be tested is very important, especially for c files.
#include <mem_checker/interface_4user.h>

// mockcpp.lib has been already included in project.
//#pragma comment(lib, "../3rdparty/mockcpp/src/Debug/mockcpp.lib")

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
		unsigned char *p = new unsigned char[10]; // should report memory leak
		
		*p = 0xff;
        *(p+1) = 0xfe;
        *(p+2) = 0x13;
        *(p+3) = 0x21;
        *(p+4) = 0x00;
        *(p+5) = 0x01;
        *(p+6) = 0x08;
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

    TEST(can report more than one leak)
    {
        char *p = new char[4]; // should not report memory leak     
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

    TEST(user can mock malloc)
    {
#if 0
#undef  malloc
        // TODO: why link error???
        MOCKER(malloc)
            .stubs()
            .will(returnValue(0));
        ASSERT_EQ((void *)0, malloc(123));

        GlobalMockObject::verify(); // TODO: link error! LNK2019: undefined symbol
        GlobalMockObject::reset();

#define malloc(size) debug_malloc(__FILE__, __LINE__, size)
#endif
    }
};

