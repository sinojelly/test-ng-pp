
IF(TESTNGPP_DISABLE_SANDBOX)
  ADD_DEFINITIONS(-DTESTNGPP_DISABLE_SANDBOX=1)
ELSE(TESTNGPP_DISABLE_SANDBOX)
  ADD_DEFINITIONS(-DTESTNGPP_DISABLE_SANDBOX=0)
ENDIF(TESTNGPP_DISABLE_SANDBOX)

ADD_DEFINITIONS(
    -DPACKAGE_NAME=${PACKAGE_NAME}
    -DPACKAGE_STRING=${PACKAGE_STRING}
    -DPACKAGE_BUGREPORT=${PACKAGE_BUGREPORT}
    -DPACKAGE_URL=${PACKAGE_URL}
    -DPACKAGE=${PACKAGE}
    -DVERSION=${VERSION}
)

SET(TESTNGPP_LIBRARY -ltestngpp)

IF(CYGWIN)
  SET(MODULE_FLAGS
    "-shared -nostdlib -lstdc++ -lgcc \
     -lcygwin -luser32 -lkernel32 \
     -ladvapi32 -lshell32 -lgcc \
     -Wl,--enable-autoimage-base \
     -Xlinker --out-implib -Xlinker"
  )
  SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${MODULE_FLAGS})
  SET(CMAKE_MODULE_LIBRARY_LINK_CXX_FLAGS ${MODULE_FLAGS})
ENDIF(CYGWIN)

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/src .)

SET(SANDBOX_SRCS
  TestCaseSandboxResultReporter.cpp 
  TestCaseSandboxResultDecoder.cpp 
  Sandbox.cpp 
  SandboxProcessor.cpp 
  TestCaseSandboxHandler.cpp 
  TestCaseSandbox.cpp 
  PipeReadableChannel.cpp 
  PipeWrittableChannel.cpp  
  TestFixtureSandboxRunner.cpp 
)

SET(TESTNGRUNNER_LIB_SRCS
  TestSuiteRunner.cpp 
  SimpleTestCaseRunner.cpp 
  TestFixtureRunnerWrapper.cpp 
  TestSuiteLoader.cpp 
  SimpleTestResultDispatcher.cpp 
  SimpleTestResultReporter.cpp 
  SimpleTestCaseResultReporter.cpp 
  SimpleTestSuiteResultReporter.cpp
  TestFixtureRunnerFactory.cpp 
  SimpleTestFixtureRunner.cpp 
  TestListenerLoader.cpp 
  WildcardTestFilter.cpp 
  OrCompositeTestFilter.cpp 
  GeneralTestFilter.cpp 
  TestFilterFactory.cpp 
  PosixFNMatcher.cpp 
  TestRunner.cpp
)

IF(NOT TESTNGPP_DISABLE_SANDBOX)
  SET(TESTNGRUNNER_LIB_SRCS
     ${TESTNGRUNNER_LIB_SRCS}
     ${SANDBOX_SRCS}
  )
ENDIF(NOT TESTNGPP_DISABLE_SANDBOX)

IF(TESTNGPP_USE_DLOPEN)
  SET(TESTNGRUNNER_LIB_SRCS ${TESTNGRUNNER_LIB_SRCS}
      DLModuleLoader.cpp 
      DLModuleLoaderFactory.cpp
  )
ELSEIF(TESTNGPP_USE_LTDL)
  SET(TESTNGRUNNER_LIB_SRCS ${TESTNGRUNNER_LIB_SRCS}
      LTModuleLoader.cpp 
      LTModuleLoaderFactory.cpp
  )
ELSE(TESTNGPP_USE_DLOPEN)
  MESSAGE(FATAL_ERROR "No module loader found")
ENDIF(TESTNGPP_USE_DLOPEN)

ADD_LIBRARY(testngpp-runner-lib SHARED ${TESTNGRUNNER_LIB_SRCS})

TARGET_LINK_LIBRARIES(testngpp-runner-lib ${TESTNGPP_LIBRARY})

ADD_EXECUTABLE(testngpp-runner TestRunnerMain.cpp)

# -Wl,--export-dynamic
ADD_DEPENDENCIES(testngpp-runner testngpp-runner-lib)
TARGET_LINK_LIBRARIES(testngpp-runner ${TESTNGPP_LIBRARY} testngpp-runner-lib)

IF(TESTNGPP_USE_DLOPEN)
  TARGET_LINK_LIBRARIES(testngpp-runner-lib -ldl)
ELSEIF(TESTNGPP_USE_LTDL)
  TARGET_LINK_LIBRARIES(testngpp-runner-lib -lltdl)
ELSE(TESTNGPP_USE_DLOPEN)
  MESSAGE(FATAL_ERROR "No module loader found")
ENDIF(TESTNGPP_USE_DLOPEN)
