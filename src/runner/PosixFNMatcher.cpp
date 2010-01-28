
#ifndef _MSVC_VER
#include <fnmatch.h>
#endif

#include <testngpp/runner/PosixFNMatcher.h>

TESTNGPP_NS_START

////////////////////////////////////////////////////////
PosixFNMatcher::
PosixFNMatcher(const std::string& namePattern)
   : pattern(namePattern)
{
}

////////////////////////////////////////////////////
namespace
{
#ifndef _MSVC_VER
   const int flags = FNM_CASEFOLD; 
#endif
}

////////////////////////////////////////////////////
bool PosixFNMatcher::
matches(const std::string& name) const
{
  if(name.size() == 0)
  {
     return false;
  }

#ifdef _MSVC_VER
  retur  true;
#else
  return ::fnmatch(pattern.c_str(), name.c_str(), flags) == 0;
#endif
}

////////////////////////////////////////////////////////

TESTNGPP_NS_END
