
#include <testngppst/internal/TestCase.h>
#include <testngppst/runner/loaders/ModuleLoader.h>

#include <mem_checker/interface_4xunit.h>


TESTNGPPST_NS_START

namespace
{

struct Reporter
{
    Reporter(TestFixture *_fixture)
        : fixture(_fixture)
    {
    }

protected:
    TestFixture *fixture;
};

struct InfoReporter : public Reporter
{
    InfoReporter(TestFixture *_fixture) : Reporter(_fixture) {}
    
    void operator ()(const char *file, unsigned int line, const char *message)
    {
        fixture->reportMemLeakInfo(file, line, message);
    }
};

struct FailureReporter : public Reporter
{
    FailureReporter(TestFixture *_fixture) : Reporter(_fixture) {}

    void operator ()(const char *file, unsigned int line, const char *message)
    {
        fixture->reportMemLeakFailure(file, line, message, false);
    }
};

}

namespace {
mem_checker::Reporter * info;
mem_checker::Reporter * failure;
}

void TestCase::startMemChecker()
{
    typedef void (*start_t)(mem_checker::Reporter *, mem_checker::Reporter *);
    start_t starter = (start_t)loader->findSymbol("startMemChecker");

	TestFixture *fixture = getFixture();
	
	info = mem_checker::createReporter(InfoReporter(fixture));
	failure = mem_checker::createReporter(FailureReporter(fixture)); // Note: Reporter is new in runner.exe, and used in .dll. is this ok?
	starter(info, failure);
}

void TestCase::verifyMemChecker()
{    
	typedef void (*verify_t)(void);    
	verify_t verifier = (verify_t)loader->findSymbol("verifyMemChecker");    
	verifier(); 

	delete info;
	delete failure;
	info = 0;
	failure = 0;
}

TESTNGPPST_NS_END






