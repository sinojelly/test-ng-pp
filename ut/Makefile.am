
CXXTEST_ROOT = $(top_builddir)/3rdparty/cxxtest

AM_CPPFLAGS = \
  -I$(top_builddir)/include \
  -I$(top_builddir)/include/testngpp \
  -I$(top_builddir)/include/testngpp/internal \
  -I$(top_builddir)/include/testngpp/runner \
  -I$(top_builddir)/include/testngpp/comm \
  -I$(top_builddir)/3rdparty \
  -I$(top_builddir)/3rdparty/mockcpp/3rdparty \
  -I$(top_builddir)/3rdparty/mockcpp/include \
  -I$(top_builddir)/3rdparty/mockcpp/include/mockcpp \
  -I$(CXXTEST_ROOT) \
  -I$(top_builddir)/src

AM_LDFLAGS = \
  $(top_builddir)/libs/libcxxtest.la \
  $(top_srcdir)/3rdparty/mockcpp/src/libmockcpp.la \
  $(top_builddir)/src/libtestngpp.la \
  $(top_builddir)/src/runner/libtestngpprunner.la 

if TESTNGPP_USE_DLOPEN
  AM_LDFLAGS += -ldl
else
  AM_LDFLAGS += $(LIBLTDL) 
endif

UT1_SOURCE = testngpp-ut1.cpp
UT2_SOURCE = testngpp-ut2.cpp
UT3_SOURCE = testngpp-ut3.cpp
UT4_SOURCE = testngpp-ut4.cpp
UT5_SOURCE = testngpp-ut5.cpp
UT6_SOURCE = testngpp-ut6.cpp

noinst_PROGRAMS = \
  testngpp-ut1 \
  testngpp-ut2 \
  testngpp-ut3 \
  testngpp-ut4 \
  testngpp-ut5 \
  testngpp-ut6

testngpp_ut1_SOURCES = $(UT1_SOURCE)
testngpp_ut2_SOURCES = $(UT2_SOURCE)
testngpp_ut3_SOURCES = $(UT3_SOURCE)
testngpp_ut4_SOURCES = $(UT4_SOURCE)
testngpp_ut5_SOURCES = $(UT5_SOURCE)
testngpp_ut6_SOURCES = $(UT6_SOURCE)

testngpp_ut6_LDADD = \
  $(top_builddir)/src/listeners/libtestngppxmllistener.la

CLEANFILES = \
  $(UT1_SOURCE) \
  $(UT2_SOURCE) \
  $(UT3_SOURCE) \
  $(UT4_SOURCE) \
  $(UT5_SOURCE) \
  $(UT6_SOURCE) 

UTCG = $(CXXTEST_ROOT)/cxxtestgen.pl

TESTSUITES1 = \
  TestTestCase.h \
  TestAssertionFailure.h \
  TestTestFixtureDesc.h \
  TestTestSuiteDesc.h \
  TestStringToOptions.h \
  TestExceptionKeywords.h

TESTSUITES2 = \
  TestTestFixtureSandboxRunner.h \
  TestSimpleTestSuiteResultReporter.h \
  TestSimpleTestCaseRunner.h 

TESTSUITES3 = \
  TestTestCaseSandboxResultReporter.h \
  TestPipeReadableChannel.h \
  TestPipeWrittableChannel.h \
  TestSandboxProcessor.h

TESTSUITES4 = \
  TestPosixFNMatcher.h \
  TestGeneralTestFilter.h \
  TestTestFilterFactory.h \
  TestOrCompositeTestFilter.h 

TESTSUITES5 = \
  TestSandbox.h  \
  TestTestCaseSandbox.h \
  TestTestSuiteRunner.h 

TESTSUITES6 = \
  TestXMLTestListener.h \
  TestXMLBuilder.h

$(UT1_SOURCE) : $(TESTSUITES1)
	$(UTCG) --have-eh --have-std --part -o $@ $^

$(UT2_SOURCE) : $(TESTSUITES2)
	$(UTCG) --have-eh --have-std --part -o $@ $^

$(UT3_SOURCE) : $(TESTSUITES3)
	$(UTCG) --have-eh --have-std --part -o $@ $^

$(UT4_SOURCE) : $(TESTSUITES4)
	$(UTCG) --have-eh --have-std --part -o $@ $^

$(UT5_SOURCE) : $(TESTSUITES5)
	$(UTCG) --have-eh --have-std --part -o $@ $^

$(UT6_SOURCE) : $(TESTSUITES6)
	$(UTCG) --have-eh --have-std --part -o $@ $^
