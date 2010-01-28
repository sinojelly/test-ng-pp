
#include <string>
#include <windows.h>

#include <testngpp/Error.h>
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/runner/Win32DllModuleLoader.h>


TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct Win32DllModuleLoaderImpl
{
    Win32DllModuleLoaderImpl();
    ~Win32DllModuleLoaderImpl();

    const char* addSearchingPaths(
        const std::list<std::string>& searchingPaths);

    void load(const std::string& modulePath);
    void loadUnderPaths( \
       const std::list<std::string>& searchingPaths, \
       const std::string& modulePath);

    void unload();

    void* findSymbol(const std::string& symbol);

    HMODULE handle;
};

////////////////////////////////////////////////////////
Win32DllModuleLoaderImpl::Win32DllModuleLoaderImpl()
    : handle(0)
{
}
////////////////////////////////////////////////////////
Win32DllModuleLoaderImpl::~Win32DllModuleLoaderImpl()
{
   unload();
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
      std::string module = modulePath + ".dll";
      return ::LoadLibrary(module.c_str());
   }
}

/////////////////////////////////////////////////////////////////
void
Win32DllModuleLoaderImpl::loadUnderPaths \
                 ( const std::list<std::string>& searchingPaths
                 , const std::string& modulePath)
{
   std::list<std::string>::const_iterator i = searchingPaths.begin();
   for(; i != searchingPaths.end(); i++)
   {
      handle = openModule(getFullPath(*i, modulePath));
      if(handle != 0)
      {
         return;
      }
   }

   load(modulePath);
}

////////////////////////////////////////////////////////
void
Win32DllModuleLoaderImpl::load(const std::string& modulePath)
{
    handle = openModule(modulePath.c_str());
    if(handle == 0)
    {
        throw Error("Can't open module " + modulePath);
    }
}

////////////////////////////////////////////////////////
void
Win32DllModuleLoaderImpl::unload()
{
    if(handle != 0)
    {
        ::FreeLibrary((HMODULE)handle);
        handle = 0;
    }
}

////////////////////////////////////////////////////////
void*
Win32DllModuleLoaderImpl::findSymbol(const std::string& symbol)
{
    if(handle == 0)
    {
        throw Error("module has not been loaded yet");
    }

    void* ptr = (void*) ::GetProcAddress((HMODULE)handle, symbol.c_str());
    if(ptr == 0)
    {
        throw Error("Can't find symbol " + symbol);
    }

    return ptr;
}

////////////////////////////////////////////////////////
Win32DllModuleLoader::Win32DllModuleLoader()
    : This(new Win32DllModuleLoaderImpl())
{
}

////////////////////////////////////////////////////////
Win32DllModuleLoader::~Win32DllModuleLoader()
{
    delete This;
}

////////////////////////////////////////////////////////
void
Win32DllModuleLoader::load(const std::string& modulePath)
{
    This->load(modulePath);
}

////////////////////////////////////////////////////////
void
Win32DllModuleLoader::loadUnderPaths( \
       const std::list<std::string>& searchingPaths, \
       const std::string& modulePath)
{
    This->loadUnderPaths(searchingPaths, modulePath);
}

////////////////////////////////////////////////////////
void
Win32DllModuleLoader::unload()
{
    This->unload();
}

////////////////////////////////////////////////////////
void*
Win32DllModuleLoader::findSymbol(const std::string& symbol)
{
   return This->findSymbol(symbol);
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END

