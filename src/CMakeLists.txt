
SET(TESTNGPP_RC_SRCS
  ResourceCheckPoint.cpp 
)

SET(TESTNGPP_SRCS
  AssertionFailure.cpp 
  TestSuiteDesc.cpp 
  TypeString.cpp 
  Formatter.cpp 
  OptionList.cpp 
  StringToOptions.cpp
)



IF(NOT TESTNGPP_DISABLE_RC)
  SET(TESTNGPP_SRCS
     ${TESTNGPP_SRCS}
     ${TESTNGPP_RC_SRCS}
  )
ENDIF(NOT TESTNGPP_DISABLE_RC)

IF(MSVC)
  SET(TESTNGPP_SRCS GetOpt.c)
ENDIF(MSVC)

ADD_LIBRARY(testngpp SHARED ${TESTNGPP_SRCS})

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/include)

