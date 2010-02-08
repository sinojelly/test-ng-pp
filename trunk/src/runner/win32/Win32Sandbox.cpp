
#include <sstream>

#include <testngpp/Error.h>
#include <testngpp/ExceptionKeywords.h>

#include <testngpp/win32/Win32Sandbox.h>
#include <testngpp/win32/Win32ThrowLastError.h>
#include <testngpp/internal/TestCase.h>


TESTNGPP_NS_START

////////////////////////////////////////////////////////
struct Win32SandboxImpl
{
   Win32SandboxImpl() : dead(false) {}

   ~Win32SandboxImpl()
   { ::CloseHandle(channelId); }

   HANDLE getSandboxId() const
   { return sandboxId; }

   HANDLE getChannelId() const
   { return channelId; }
   
   HANDLE getEventId() const
   { return eventId; }

   HANDLE sandboxId;
   HANDLE channelId;
   HANDLE eventId;
   bool dead;
};

////////////////////////////////////////////////////////
Win32Sandbox::Win32Sandbox()
   : This(new Win32SandboxImpl())
{}

////////////////////////////////////////////////////////
Win32Sandbox::~Win32Sandbox()
{
   delete This;
}

////////////////////////////////////////////////////////
HANDLE
Win32Sandbox::getChannelId() const
{
   return This->getChannelId();
}

////////////////////////////////////////////////////////
HANDLE
Win32Sandbox::getSandboxId() const
{
   return This->getSandboxId();
}

////////////////////////////////////////////////////////
HANDLE
Win32Sandbox::getEventId() const
{
   return This->getEventId();
}

////////////////////////////////////////////////////////
void
Win32Sandbox::cleanup()
{
	::CloseHandle(getSandboxId());
	::CloseHandle(getChannelId());
	::CloseHandle(getEventId());
}

/////////////////////////////////////////////////////////
bool
Win32Sandbox::isDead() const
{
   return This->dead;
}

/////////////////////////////////////////////////////////
void
Win32Sandbox::die()
{
  This->dead = true;
}

namespace
{
   void pipe(HANDLE pipes[])
   {
	  SECURITY_ATTRIBUTES secAttr;
      secAttr.bInheritHandle = TRUE;
      secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
      secAttr.lpSecurityDescriptor = NULL;

      HANDLE hRead;
      if(!::CreatePipe
	        ( &hRead
	        , &pipes[1]
			, &secAttr
			, 1024))
	  {
	      throwLastError();
	  }
	  
      if(!::DuplicateHandle
	        ( GetCurrentProcess()
			, hRead
			, GetCurrentProcess()
			, &pipes[0]
			, FILE_GENERIC_READ
			, FALSE  // Should not be inherited
			, DUPLICATE_CLOSE_SOURCE))
      {
	     ::CloseHandle(pipes[1]);
		 throwLastError();
      }	 
   }
   
   std::string getModulePath()
   {
      char szModulePath[512] = {0};
      if(!::GetModuleFileName(NULL, szModulePath, sizeof(szModulePath)))
	  {
	     throwLastError();
	  }

	  *(strrchr(szModulePath, '\\')) = '\0';
	  
	  return std::string(szModulePath);
   }
   
   HANDLE
   createSandBoxProcess
         ( const std::string& suite
		 , const std::string& fixture
		 , const std::string& testcase
		 , HANDLE hWrite
		 , HANDLE hSemphore)
   {
	  std::string modulePath = getModulePath() + "\\testngpp-win32-testcase-runner.exe";
	  std::stringstream oss;
	  
	  oss << "\"" << modulePath << "\"" 
		  << " "  << suite 
          << " "  << fixture 
          << " "  << testcase 
          << " "  << (DWORD)hWrite
          << " "  << (DWORD)hSemphore;
      
	  STARTUPINFO startInfo;
	  GetStartupInfo(&startInfo);
	  
	  PROCESS_INFORMATION prcInfo;
	  if(!CreateProcess
	        ( (LPSTR)modulePath.c_str()
	        , (LPSTR)oss.str().c_str()
			, 0
			, 0
			, TRUE  // bInheritHandles
			, CREATE_NO_WINDOW
			, 0
			, 0
			, &startInfo
			, &prcInfo))
      {
	     throwLastError();
      }
      
	  HANDLE hChild = ::OpenProcess
		    ( SYNCHRONIZE | PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE
			, FALSE
			, prcInfo.dwProcessId); 
	  if(0 == hChild)
	  {
	     throwLastError();
	  }
	  
	  if(WAIT_FAILED == ::WaitForInputIdle(hChild, INFINITE))
	  {
	     throwLastError();
	  }
	  
	  return hChild;
   }
   
   HANDLE createSemaphore()
   {
      SECURITY_ATTRIBUTES secAttr;
      secAttr.bInheritHandle = TRUE;
      secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
      secAttr.lpSecurityDescriptor = 0;
	  
      HANDLE handle = \
	     ::CreateSemaphore
		    ( &secAttr
			, 0
			, 65535
			, 0);
      if(0 == handle)
	  {
	     throwLastError();
	  }
	  
	  return handle;
   }
}
////////////////////////////////////////////////////////
Win32Sandbox*
Win32Sandbox::createInstance(const TestCase* testcase)
{
   HANDLE sockets[2];

   pipe(sockets);
   
   HANDLE hSemaphore = 0;
   
   __TESTNGPP_TRY
   {
      hSemaphore = createSemaphore();
   }
   __TESTNGPP_CATCH_ALL
   {
      ::CloseHandle(sockets[0]);
	  ::CloseHandle(sockets[1]);
	  throw;
   }
   __TESTNGPP_END_TRY
	
   HANDLE hSandbox = 0;
   
   __TESTNGPP_TRY
   {
      hSandbox = \
         createSandBoxProcess
            ( testcase->getNameOfSuite()
		    , testcase->getNameOfFixture()
		    , testcase->getName()
		    , sockets[1]
		    , hSemaphore);
   }
   __TESTNGPP_CATCH_ALL
   {
      ::CloseHandle(sockets[0]);
	  ::CloseHandle(hSemaphore);
   }
   __TESTNGPP_FINALLY
   {
      ::CloseHandle(sockets[1]);
   }
   __TESTNGPP_DONE

   Win32Sandbox * sandbox = new Win32Sandbox();

   sandbox->This->channelId = sockets[0];
   sandbox->This->sandboxId = hSandbox;
   sandbox->This->eventId   = hSemaphore;

   return sandbox;
}

TESTNGPP_NS_END

