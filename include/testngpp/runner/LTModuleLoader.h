
#ifndef __TESTNGPP_LT_MODULE_LOADER_H
#define __TESTNGPP_LT_MODULE_LOADER_H

#include <testngpp/testngpp.h>
#include <testngpp/runner/ModuleLoader.h>


TESTNGPP_NS_START

struct LTModuleLoaderImpl;

struct LTModuleLoader : public ModuleLoader
{
   LTModuleLoader();
   ~LTModuleLoader(); 

	void load(const std::string& modulePath);

	void loadUnderPaths( \
       const std::list<std::string>& searchingPaths, \
       const std::string& modulePath);

   void unload();

   void* findSymbol(const std::string& symbol);

private:

   LTModuleLoaderImpl* This;
};

TESTNGPP_NS_END

#endif

