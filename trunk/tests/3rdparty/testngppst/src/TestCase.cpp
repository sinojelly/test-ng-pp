
#include <testngppst/internal/TestCase.h>
#include <testngppst/runner/loaders/ModuleLoader.h>

#include <testngppst/internal/Error.h>
#include <mem_checker/interface_4xunit.h>

TESTNGPPST_NS_START

namespace
{
void reportMemoryLeak(const char *message)
{
    throw Error(message);
}
}

void TestCase::startMemChecker()
{
    typedef void (*start_t)(report_t);
    start_t starter = (start_t)loader->findSymbol("startMemChecker");
    starter(reportMemoryLeak);
}

void TestCase::verifyMemChecker()
{
    typedef void (*verify_t)(void);
    verify_t verifier = (verify_t)loader->findSymbol("verifyMemChecker");
    verifier(); 
}


TESTNGPPST_NS_END






