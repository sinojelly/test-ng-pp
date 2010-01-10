

#include <string>
#include <sstream>
#include <iostream>

#include <testngpp/Error.h>
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/utils/StringToOptions.h>

#include <testngpp/runner/TestListener.h>
#include <testngpp/runner/TestRunnerContext.h>
#include <testngpp/runner/LTTestListenerLoader.h>
#include <testngpp/runner/ModuleLoader.h>


TESTNGPP_NS_START

struct LTTestListenerLoaderImpl
{
   LTTestListenerLoaderImpl
       ( ModuleLoader* moduleLoader
       , const std::string& args);

	~LTTestListenerLoaderImpl();

   void destroyArgs();
   void destroyListener();

   void doLoad(TestRunnerContext* context);
   void load( TestRunnerContext* context
            , const std::list<std::string>& searchingPaths);

   std::string name;
   StringToOptions::CArgs args; 
   
   TestListener* listener;
   ModuleLoader* loader;
};

namespace
{
   std::string getDestroySymbolName(const std::string& name)
   {
      std::stringstream ss;
      ss << name << "_destroy_instance";
      return ss.str();
   }
}

///////////////////////////////////////////////////////////////
LTTestListenerLoaderImpl::
LTTestListenerLoaderImpl
       ( ModuleLoader* moduleLoader
       , const std::string& cl)
   : loader(moduleLoader)
   , listener(0)
{
   args = StringToOptions::parse(cl);

   if(args.first == 0)
   {
      name = "";
      return;
   }

   name = args.second[0];
}

///////////////////////////////////////////////////////////////
void
LTTestListenerLoaderImpl::destroyListener()
{
   typedef void (*TestListenerDestroy)(TestListener*);
   TestListenerDestroy destroy = 0;

   __TESTNGPP_TRY
   {
      destroy =  (TestListenerDestroy) \
         loader->findSymbol(getDestroySymbolName(name));

      destroy(listener);
   }
   __TESTNGPP_CATCH_ALL
   {
      delete listener;
   }
   __TESTNGPP_FINALLY
   {
      listener = 0;
      loader->unload();
   }
   __TESTNGPP_DONE

}

///////////////////////////////////////////////////////////////
void
LTTestListenerLoaderImpl::destroyArgs()
{
   for(int i = 0; i < args.first; i++)
   {
      delete [] args.second[i];
   }

   delete [] args.second;
}

///////////////////////////////////////////////////////////////
LTTestListenerLoaderImpl::~LTTestListenerLoaderImpl()
{
   destroyArgs();

   if(listener != 0)
   {
      destroyListener();
   }

   if(loader != 0)
   {
      loader->unload();
      delete loader;
   }
}

///////////////////////////////////////////////////////////////
LTTestListenerLoader::
LTTestListenerLoader(ModuleLoader* loader, const std::string& path)
   : This(new LTTestListenerLoaderImpl(loader, path))
{
}

///////////////////////////////////////////////////////////////
LTTestListenerLoader::~LTTestListenerLoader()
{
   delete This;
}

namespace
{
   std::string getListenerSharedObjectName(const std::string& name)
   {
      std::stringstream ss;
      ss << "lib" << name;
      return ss.str();
   }

   std::string getCreaterSymbolName(const std::string& name)
   {
      std::stringstream ss;
      ss << name << "_create_instance";
      return ss.str();
   }
}

///////////////////////////////////////////////////////////////
void 
LTTestListenerLoaderImpl::
load( TestRunnerContext* context
    , const std::list<std::string>& searchingPaths)
{
   loader->loadUnderPaths( searchingPaths
                         , getListenerSharedObjectName(name));

   typedef TestListener* (*TestListenerCreater) \
                  ( TestResultReporter* \
						, TestSuiteResultReporter* \
						, TestCaseResultReporter* \
						, int, char**);

   TestListenerCreater create = (TestListenerCreater) \
       loader->findSymbol(getCreaterSymbolName(name));

   listener = create( context->getTestResultReporter()
                    , context->getTestSuiteResultReporter()
                    , context->getTestCaseResultReporter()
                    , args.first
                    , args.second);
   if(listener == 0)
   {
      throw Error("Cannot create listener");
   }

   context->registerTestListener(listener);
}

/////////////////////////////////////////////////////////////////
void
LTTestListenerLoader::
load( TestRunnerContext* context
    , const std::list<std::string>& searchingPaths)
{
   std::cout << "loading " << This->name << " ... "; std::cout.flush();

   __TESTNGPP_TRY
   {
      This->load(context, searchingPaths);
   }
   __TESTNGPP_CATCH(Error& e)
   {
      std::cerr << e.what() << std::endl;
      throw;
   }
   __TESTNGPP_END_TRY

   std::cout << "OK" << std::endl;
}

/////////////////////////////////////////////////////////////////

TESTNGPP_NS_END

