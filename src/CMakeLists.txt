
IF(MSVC)
  ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )
ENDIF(MSVC)

SET(TESTNGPP_SRCS
  except/Exception.cpp
  except/AssertionFailure.cpp 
  TestFixture.cpp
  TestSuiteDesc.cpp 
  TypeString.cpp 
  Formatter.cpp 
  TestCase.cpp
  mem_checker/bool_array.cpp       
  mem_checker/mem_pool_base.cpp
  mem_checker/debug_new.cpp        
  mem_checker/static_mem_pool.cpp
  mem_checker/interface_4xunit.cpp
  mem_checker/reporter.cpp
)

# include debug info in testngpp.lib
IF(MSVC)
	ADD_DEFINITIONS( /Z7 )
ENDIF(MSVC)

IF(MSVC)
  ADD_LIBRARY(testngpp STATIC ${TESTNGPP_SRCS})
ELSE(MSVC)  
  ADD_LIBRARY(testngpp SHARED ${TESTNGPP_SRCS})
ENDIF(MSVC)

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/3rdparty)

IF(MSVC)
  INSTALL(
   TARGETS testngpp
   ARCHIVE DESTINATION lib)
ELSE(MSVC)  
  INSTALL(
   TARGETS testngpp
   LIBRARY DESTINATION lib)
ENDIF(MSVC)

   


