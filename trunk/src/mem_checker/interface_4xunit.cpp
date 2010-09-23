/*
 * Add by Chen Guodong <sinojelly@gmail.com>
 *
 */

/**
 * @file interface_4xunit.cpp
 *
 *
 * @version 1.0, 2010/9/22
 * @author  sinojelly
 *
 */
 
#include <mem_checker/interface_4xunit.h>

extern report_t memory_leak_reporter;
extern int check_leaks();

bool g_need_check = false; // let debug_new.cpp call this file , avoid it not linked into dll.

extern "C" __declspec(dllexport) void startMemChecker(report_t reporter)
{
    if (0 != reporter)
	{
		memory_leak_reporter = reporter;
	}

    g_need_check = true;
}

extern "C" __declspec(dllexport) void verifyMemChecker()
{
	check_leaks();
    g_need_check = false;
}

extern "C" void stopMemChecker()
{
    g_need_check = false;
}

