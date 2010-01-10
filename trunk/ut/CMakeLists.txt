
INCLUDE(FindPythonInterp)

IF(PYTHONINTERP_FOUND)
  SET(PYTHON ${PYTHON_EXECUTABLE})
ELSE(PYTHONINTERP_FOUND)
  MESSAGE(FATAL_ERROR "No Python interpreter found")
ENDIF(PYTHONINTERP_FOUND)

ADD_DEFINITIONS(
    -DPACKAGE_NAME=${PACKAGE_NAME}
    -DPACKAGE_STRING=${PACKAGE_STRING}
    -DPACKAGE_BUGREPORT=${PACKAGE_BUGREPORT}
    -DPACKAGE_URL=${PACKAGE_URL}
    -DPACKAGE=${PACKAGE}
    -DVERSION=${VERSION}
)

SET(TESTNGPP_LIBRARY -ltestngpp)

IF(CMAKE_SYSTEM_NAME MATCHES Linux)
  IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_MODULE_LINKER_FLAGS "-Wl,--fatal-warnings -Wl,--no-undefined")
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDIF(CMAKE_SYSTEM_NAME MATCHES Linux)

SET(UT_CASES
  TestTestCase.h 
  TestAssertionFailure.h 
  TestTestFixtureDesc.h 
  TestTestSuiteDesc.h 
  TestStringToOptions.h 
  TestExceptionKeywords.h
  TestTestFixtureSandboxRunner.h 
  TestSimpleTestSuiteResultReporter.h 
  TestSimpleTestCaseRunner.h
  TestTestCaseSandboxResultReporter.h 
  TestPipeReadableChannel.h 
  TestPipeWrittableChannel.h 
  TestSandboxProcessor.h
  TestPosixFNMatcher.h 
  TestGeneralTestFilter.h 
  TestTestFilterFactory.h 
  TestOrCompositeTestFilter.h
  TestSandbox.h 
  TestTestCaseSandbox.h 
  TestTestSuiteRunner.h
  TestXMLTestListener.h 
  TestXMLBuilder.h
)

LINK_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/libs
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/listeners
  ${CMAKE_SOURCE_DIR}/src/runner 
  ${CMAKE_SOURCE_DIR}/3rdparty/mockcpp/src)

INCLUDE_DIRECTORIES(BEFORE 
  .
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include/testngpp
  ${CMAKE_SOURCE_DIR}/include/testngpp/internal
  ${CMAKE_SOURCE_DIR}/include/testngpp/runner
  ${CMAKE_SOURCE_DIR}/include/testngpp/comm
  ${CMAKE_SOURCE_DIR}/include/testngpp/utils
  ${CMAKE_SOURCE_DIR}/3rdparty/cxxtest
  ${CMAKE_SOURCE_DIR}/3rdparty/mockcpp/3rdparty
  ${CMAKE_SOURCE_DIR}/3rdparty/mockcpp/include
  )

SET(TEST_GENERATOR ${CMAKE_SOURCE_DIR}/3rdparty/cxxtest/cxxtestgen.py --have-eh --have-std --part )
SET(TESTCASE_PREFIX testngpp-ut)
SET(INDEX 0)

FOREACH(CASE ${UT_CASES})
  GET_FILENAME_COMPONENT(CASE_NAME ${CASE} NAME_WE)
  SET(CASE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${CASE_NAME}.cpp)
  MATH(EXPR INDEX "${INDEX}+1")

  ADD_CUSTOM_TARGET(${CASE_NAME} DEPENDS ${CASE_SRC})

  SET(CASE_MODULE ${TESTCASE_PREFIX}-${INDEX})

  ADD_EXECUTABLE(${CASE_MODULE} ${CASE_SRC})

  TARGET_LINK_LIBRARIES(${CASE_MODULE}
    -lcxxtest 
    -lmockcpp 
    -ltestngpp 
    -ltestngpp-runner-lib 
    -ltestngppxmllistener-fortest)

  ADD_CUSTOM_COMMAND(
    OUTPUT ${CASE_SRC}
    COMMAND ${PYTHON} ${TEST_GENERATOR} -o ${CASE_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/${CASE}
  )

  ADD_DEPENDENCIES(${CASE_MODULE} ${CASE_NAME})
ENDFOREACH()



