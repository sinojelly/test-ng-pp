
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

SET(UT_CASES
  TestAssertionFailure.h
  TestModuleTestSuiteLoader.h
  TestPipeReadableChannel.h
  TestPipeWrittableChannel.h
  TestPosixFNMatcher.h
  TestStringToOptions.h
  TestTagsParser.h
  TestTestCase.h
  TestTestFixtureDesc.h
  TestTestSuiteDesc.h
  TestXMLBuilder.h
  TestDataDriven.h
)

SET(UT_CASES_WITH_MOCK
  TestTestFixtureSandboxRunner.h
  TestTestCaseHierarchy.h
  TestSandbox.h
  TestSandboxProcessor.h
  TestTestSuiteContext.h
  TestTestCaseSandboxResultReporter.h
  TestXMLTestListener.h
  TestTestFilterFactory.h
  TestOrCompositeTestFilter.h
  TestSimpleTestSuiteResultReporter.h
  TestGeneralTestFilter.h
)

SET(UT_CASES ${UT_CASES} ${UT_CASES_WITH_MOCK})

LINK_DIRECTORIES(
  ${PROJECT_HOME}/src
  ${PROJECT_HOME}/src/utils
  ${PROJECT_HOME}/src/listeners
  ${PROJECT_HOME}/src/runner
  ${CMAKE_BINARY_DIR}/3rdparty/testngppst/src)

INCLUDE_DIRECTORIES(BEFORE 
  .
  ${PROJECT_HOME}/include
  ${PROJECT_HOME}/3rdparty
  ${PROJECT_HOME}/src
  ${CMAKE_SOURCE_DIR}/3rdparty/testngppst/include
  ${CMAKE_SOURCE_DIR}/3rdparty/mockcpp/include
  )

SET(TEST_GENERATOR ${CMAKE_SOURCE_DIR}/3rdparty/testngppst/scripts/testngppstgen.py)
SET(TESTCASE_PREFIX testngpp-ut)

FOREACH(CASE ${UT_CASES})
  GET_FILENAME_COMPONENT(CASE_NAME ${CASE} NAME_WE)
  SET(CASE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${CASE_NAME}.cpp)

  ADD_CUSTOM_TARGET(${CASE_NAME} DEPENDS ${CASE_SRC})

  SET(CASE_MODULE ${TESTCASE_PREFIX}-${CASE_NAME})

  ADD_LIBRARY(${CASE_MODULE} MODULE ${CASE_SRC})

  TARGET_LINK_LIBRARIES(${CASE_MODULE}
    testngppst
    testngpp-utils
    testngpp
    testngpp-runner-lib
    testngppxmllistener-fortest
    mockcpp)

  ADD_CUSTOM_COMMAND(
    OUTPUT ${CASE_SRC}
    COMMAND ${PYTHON} ${TEST_GENERATOR} -o ${CASE_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/${CASE}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${CASE}
  )

  ADD_DEPENDENCIES(${CASE_MODULE} ${CASE_NAME})
ENDFOREACH()


