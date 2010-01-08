
TESTNGPP_INCLUDE_PATH = $(top_builddir)/include/testngpp

AM_CPPFLAGS = -I$(top_builddir)/include 
AM_LDFLAGS = -export-dynamic -no-undefined 

lib_LTLIBRARIES = libtestngpp.la

libtestngpp_la_SOURCES = \
  AssertionFailure.cpp \
  TestSuiteDesc.cpp \
  ResourceCheckPoint.cpp \
  TypeString.cpp \
  Formatter.cpp \
  OptionList.cpp \
  StringToOptions.cpp 

pkginclude_HEADERS = \
  $(TESTNGPP_INCLUDE_PATH)/testngpp.hpp \
  $(TESTNGPP_INCLUDE_PATH)/testngpp.h \
  $(TESTNGPP_INCLUDE_PATH)/Asserter.h \
  $(TESTNGPP_INCLUDE_PATH)/Error.h \
  $(TESTNGPP_INCLUDE_PATH)/AssertionFailure.h \
  $(TESTNGPP_INCLUDE_PATH)/TestFixture.h \
  $(TESTNGPP_INCLUDE_PATH)/ResourceCheckPoint.h 


internaldir = $(pkgincludedir)/internal

internal_HEADERS = \
  $(TESTNGPP_INCLUDE_PATH)/internal/TestCase.h \
  $(TESTNGPP_INCLUDE_PATH)/internal/TestFixtureDesc.h \
  $(TESTNGPP_INCLUDE_PATH)/internal/TestSuiteDesc.h \
  $(TESTNGPP_INCLUDE_PATH)/internal/TestCaseInfoReader.h \
  $(TESTNGPP_INCLUDE_PATH)/internal/TestSuiteInfoReader.h \
  $(TESTNGPP_INCLUDE_PATH)/internal/TestFixtureInfoReader.h

utilsdir = $(pkgincludedir)/utils

utils_HEADERS = \
  $(TESTNGPP_INCLUDE_PATH)/utils/TypeString.h \
  $(TESTNGPP_INCLUDE_PATH)/utils/Formatter.h  \
  $(TESTNGPP_INCLUDE_PATH)/utils/OptionList.h  

