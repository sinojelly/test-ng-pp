
SET(TESTNGPP_SRCS
  AssertionFailure.cpp 
  TestSuiteDesc.cpp 
  ResourceCheckPoint.cpp 
  TypeString.cpp 
  Formatter.cpp 
  OptionList.cpp 
  StringToOptions.cpp
)

ADD_LIBRARY(testngpp SHARED ${TESTNGPP_SRCS})

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/include)

