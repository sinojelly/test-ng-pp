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
#include <testngppst/testngppst.h>

extern int check_leaks();

static bool g_need_check = false; // let debug_new.cpp call this file , avoid it not linked into dll.
static bool report_to_xunit = true; // user can stop/open mem checker reporter to xunit

extern "C" void stopMemChecker()
{
    g_need_check = false; // the following new/malloc info will not be checked in check_leaks
    report_to_xunit = false; // report nothing to xunit 
}

extern "C" void openMemChecker()
{
    g_need_check = true;
    report_to_xunit = true;
}

bool needToCheck()
{
    return g_need_check;
}

bool needToReport()
{
    return report_to_xunit;
}

extern "C" DLL_EXPORT void startMemChecker
    ( mem_checker::Reporter *infoReporter
    , mem_checker::Reporter *failureReporter)
{
	set_reporter(infoReporter, failureReporter);
    openMemChecker();
}

extern "C" DLL_EXPORT void verifyMemChecker()
{    
    // avoid next testcase report lastcase's mem leaks
    g_need_check = false; // check_leaks may throw exception, so must set g_need_check first.
	check_leaks();
    clr_reporter();
}


