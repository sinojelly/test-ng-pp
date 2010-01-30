
#ifndef __TESTNGPP_DL_MODULE_LOADER_H
#define __TESTNGPP_DL_MODULE_LOADER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/ModuleLoader.h>


TESTNGPP_NS_START

struct StringList;
struct DLModuleLoaderImpl;

struct DLModuleLoader : public ModuleLoader
{
   DLModuleLoader(const StringList* searchingPaths);
   ~DLModuleLoader(); 

	void load(const std::string& modulePath);
   void unload();

   void* findSymbol(const std::string& symbol);

private:

   DLModuleLoaderImpl* This;
};

TESTNGPP_NS_END

#endif

