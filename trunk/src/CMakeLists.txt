
IF(MSVC)
  ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )
ENDIF(MSVC)

SET(TESTNGPP_SRCS
  Exception.cpp
  AssertionFailure.cpp 
  TestFixture.cpp
  TestSuiteDesc.cpp 
  TypeString.cpp 
  Formatter.cpp 
  OptionList.cpp 
  StringToOptions.cpp
  ConsoleState.cpp
  StupidTimer.cpp
)

IF(MSVC)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)
  SET(TESTNGPP_SRCS ${TESTNGPP_SRCS} GetOpt.c)
ENDIF(MSVC)

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

   


