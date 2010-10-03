
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

TARGET_LINK_LIBRARIES(mem-checker)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/src/mem_checker)

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

   


