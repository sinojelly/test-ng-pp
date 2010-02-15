
#include <testngppst/runner/ModuleLoader.h>
#include <testngppst/runner/ModuleLoaderFactory.h>

#include <testngppst/runner/Win32DllModuleLoader.h>


TESTNGPPST_NS_START


////////////////////////////////////////////////////////
ModuleLoader*
ModuleLoaderFactory::create()
{
    return new Win32DllModuleLoader();
}

////////////////////////////////////////////////////////

TESTNGPPST_NS_END

