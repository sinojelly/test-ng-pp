
SET(TESTNGPP_SRCS
  AssertionFailure.cpp 
  TestSuiteDesc.cpp 
  ResourceCheckPoint.cpp 
  TypeString.cpp 
  Formatter.cpp 
  OptionList.cpp 
  StringToOptions.cpp
)

IF(CYGWIN)
  SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS
    "-shared -nostdlib -lstdc++ -lgcc \
     -lcygwin -luser32 -lkernel32 \
     -ladvapi32 -lshell32 -lgcc \
     -Wl,--enable-autoimage-base \
     -Xlinker --out-implib -Xlinker"
  ) 
ENDIF(CYGWIN)

ADD_LIBRARY(testngpp SHARED ${TESTNGPP_SRCS})

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/include)

