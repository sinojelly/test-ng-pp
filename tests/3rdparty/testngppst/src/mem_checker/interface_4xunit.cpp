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
#include <mem_checker/reporter.h>

extern int check_leaks();

bool g_need_check = false; // let debug_new.cpp call this file , avoid it not linked into dll.

extern "C" __declspec(dllexport) void startMemChecker
    ( mem_checker::Reporter *infoReporter
    , mem_checker::Reporter *failureReporter)
{
	set_reporter(infoReporter, failureReporter);
    g_need_check = true;
}

extern "C" __declspec(dllexport) void verifyMemChecker()
{    
    g_need_check = false; // check_leaks may throw exception, so must set g_need_check first.
	check_leaks();
    clr_reporter();
}

extern "C" void stopMemChecker()
{
    g_need_check = false;
}

