
#include <testngpp/internal/TestCase.h>
#include <testngpp/runner/loaders/ModuleLoader.h>

#include <testngpp/internal/Error.h>
#include <mem_checker/interface_4xunit.h>

TESTNGPP_NS_START

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


TESTNGPP_NS_END






