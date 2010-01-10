
SET(TESTNGPP_SRCS
  AssertionFailure.cpp 
  TestSuiteDesc.cpp 
  ResourceCheckPoint.cpp 
  TypeString.cpp 
  Formatter.cpp 
  OptionList.cpp 
  StringToOptions.cpp
)

MESSAGE(STATUS ${CMAKE_SOURCE_DIR})

ADD_LIBRARY(testngpp SHARED ${TESTNGPP_SRCS})

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/include)

