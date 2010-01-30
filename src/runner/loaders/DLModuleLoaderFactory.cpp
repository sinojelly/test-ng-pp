
#include <testngpp/runner/ModuleLoader.h>
#include <testngpp/runner/ModuleLoaderFactory.h>

#include <testngpp/runner/DLModuleLoader.h>
#include <testngpp/runner/StringList.h>

TESTNGPP_NS_START


////////////////////////////////////////////////////////
ModuleLoader*
ModuleLoaderFactory::create(const StringList* searchingPaths)
{
    return new DLModuleLoader(searchingPaths);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END

