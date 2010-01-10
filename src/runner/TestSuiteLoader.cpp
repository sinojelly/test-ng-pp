
#include <testngpp/Error.h>

#include <testngpp/runner/LTTestSuiteLoader.h>
#include <testngpp/runner/TestSuiteDescEntryNameGetter.h>
#include <testngpp/runner/ModuleLoaderFactory.h>
#include <testngpp/runner/ModuleLoader.h>

TESTNGPP_NS_START

struct LTTestSuiteLoaderImpl
{
   LTTestSuiteLoaderImpl()
      : loader(0)
   {}
   
   ~LTTestSuiteLoaderImpl();

   ModuleLoader* getLoader();

   TestSuiteDesc* load(const std::string& path,
      TestSuiteDescEntryNameGetter* nameGetter);

   void unload();

   ModuleLoader* loader;
};

///////////////////////////////////////////////////////////////
ModuleLoader*
LTTestSuiteLoaderImpl::getLoader()
{
   if(loader == 0)
   {
      loader = ModuleLoaderFactory::create();
   }

   return loader;
}
///////////////////////////////////////////////////////////////
void
LTTestSuiteLoaderImpl::unload()
{
   if(loader != 0)
   {
      delete loader;
      loader = 0;
   }
}
///////////////////////////////////////////////////////////////
LTTestSuiteLoaderImpl::~LTTestSuiteLoaderImpl()
{
   unload();
}


///////////////////////////////////////////////////////////////
TestSuiteDesc*
LTTestSuiteLoaderImpl::
load( const std::string& path
    , TestSuiteDescEntryNameGetter* nameGetter)
{
   getLoader()->load(path);
 
   typedef TestSuiteDesc* (*TestSuiteDescGetter)();

   TestSuiteDescGetter getter = (TestSuiteDescGetter) \
       getLoader()->findSymbol(nameGetter->getDescEntryName());

   TestSuiteDesc* desc = getter();
   if(desc == 0)
   {
      throw Error("Invalid test suite shared object");
   }

   return desc;
}

///////////////////////////////////////////////////////////////
LTTestSuiteLoader::LTTestSuiteLoader()
   : This(new LTTestSuiteLoaderImpl())
{
}

///////////////////////////////////////////////////////////////
LTTestSuiteLoader::~LTTestSuiteLoader()
{
   delete This;
}

/////////////////////////////////////////////////////////////////
TestSuiteDesc*
LTTestSuiteLoader::
load( const std::string& path
    , TestSuiteDescEntryNameGetter* nameGetter)
{
   return This->load(path, nameGetter);
}

/////////////////////////////////////////////////////////////////
void LTTestSuiteLoader::unload()
{
   This->unload();
}

/////////////////////////////////////////////////////////////////

TESTNGPP_NS_END

