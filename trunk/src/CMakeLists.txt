
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
)

IF(MSVC)
  ADD_LIBRARY(testngpp STATIC ${TESTNGPP_SRCS})
ELSE(MSVC)  
  ADD_LIBRARY(testngpp SHARED ${TESTNGPP_SRCS})
ENDIF(MSVC)

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/include)

IF(MSVC)
  INSTALL(
   TARGETS testngpp
   ARCHIVE DESTINATION lib)
ELSE(MSVC)  
  INSTALL(
   TARGETS testngpp
   LIBRARY DESTINATION lib)
ENDIF(MSVC)

   


