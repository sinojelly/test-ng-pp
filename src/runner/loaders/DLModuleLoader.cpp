
#include <string>
#include <dlfcn.h>

#include <testngpp/Error.h>
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/runner/DLModuleLoader.h>
#include <testngpp/runner/StringList.h>

TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct DLModuleLoaderImpl
{
    DLModuleLoaderImpl(const StringList* searchingPaths);
    ~DLModuleLoaderImpl();


    void load(const std::string& modulePath);
    void loadUnderPaths(const std::string& modulePath);

    void unload();

    void* findSymbol(const std::string& symbol);

    StringList::Type& getSearchingPaths() const;

    const StringList* searchingPaths;
    StringList dummyPaths;

    void* handle;
};

////////////////////////////////////////////////////////
DLModuleLoaderImpl::
DLModuleLoaderImpl(const StringList* paths)
    : handle(0)
    , searchingPaths(paths)
{
}
////////////////////////////////////////////////////////
DLModuleLoaderImpl::~DLModuleLoaderImpl()
{
    if(handle != 0)
    {
        ::dlclose(handle);
        handle = 0;
    }
}

StringList::Type&
DLModuleLoaderImpl::getSearchingPaths() const
{
   if(searchingPaths == 0)
   {
      return dummyPaths.get();
   }

   return searchingPaths->get();
}

/////////////////////////////////////////////////////////////////
namespace
{
   std::string getFullPath( const std::string& path
                           , const std::string& modulePath)
   {
      std::string fullPath(path);

      if(fullPath.at(fullPath.length()-1) != '/')
      {
         fullPath += "/";
      }

      fullPath += modulePath;

      return fullPath;
   }

   void* openModule(const std::string& modulePath)
   {
      std::string module = modulePath + ".so";
      return ::dlopen(module.c_str(), RTLD_LAZY);
   }
}

/////////////////////////////////////////////////////////////////
void
DLModuleLoaderImpl::loadUnderPaths \
                 ( const std::string& modulePath)
{
   std::list<std::string>::const_iterator i = getSearchingPaths().begin();
   for(; i != getSearchingPaths().end(); i++)
   {
      handle = openModule(getFullPath(*i, modulePath));
      if(handle != 0)
      {
         return;
      }
   }

   throw Error(::dlerror());
}

////////////////////////////////////////////////////////
void
DLModuleLoaderImpl::load(const std::string& modulePath)
{
   handle = openModule(modulePath);
   if(handle == 0)
   {
      loadUnderPaths(modulePath);
   }
}

////////////////////////////////////////////////////////
void
DLModuleLoaderImpl::unload()
{
    if(handle != 0)
    {
        ::dlclose(handle);
    }
}

////////////////////////////////////////////////////////
void*
DLModuleLoaderImpl::findSymbol(const std::string& symbol)
{
    if(handle == 0)
    {
        throw Error("module has not been loaded yet");
    }

    void* ptr = (void*) ::dlsym(handle, symbol.c_str());
    if(ptr == 0)
    {
        throw Error(::dlerror());
    }

    return ptr;
}

////////////////////////////////////////////////////////
DLModuleLoader::DLModuleLoader(const StringList* searchingPaths)
    : This(new DLModuleLoaderImpl(searchingPaths))
{
}

////////////////////////////////////////////////////////
DLModuleLoader::~DLModuleLoader()
{
    delete This;
}

////////////////////////////////////////////////////////
void
DLModuleLoader::load(const std::string& modulePath)
{
    This->load(modulePath);
}

////////////////////////////////////////////////////////
void
DLModuleLoader::unload()
{
    This->unload();
}

////////////////////////////////////////////////////////
void*
DLModuleLoader::findSymbol(const std::string& symbol)
{
   return This->findSymbol(symbol);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END

