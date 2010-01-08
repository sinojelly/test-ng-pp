
CXXTEST_ROOT = $(top_builddir)/3rdparty/cxxtest

AM_CPPFLAGS = \
  -I$(top_builddir)/include \
  -I$(top_builddir)/include/testcpp \
  -I$(top_builddir)/include/testcpp/internal \
  -I$(top_builddir)/include/testcpp/runner \
  -I$(top_builddir)/include/testcpp/comm \
  -I$(top_builddir)/3rdparty \
  -I$(top_builddir)/3rdparty/mockcpp/3rdparty \
  -I$(top_builddir)/3rdparty/mockcpp/include \
  -I$(top_builddir)/3rdparty/mockcpp/include/mockcpp \
  -I$(CXXTEST_ROOT) \
  -I$(top_builddir)/src

AM_LDFLAGS = \
  $(top_builddir)/libs/libcxxtest.la \
  $(top_srcdir)/3rdparty/mockcpp/src/libmockcpp.la \
  $(top_builddir)/src/libtestcpp.la \
  $(top_builddir)/src/runner/libtestcpprunner.la \
  $(LIBLTDL) 


UT1_SOURCE = testcpp-ut1.cpp
UT2_SOURCE = testcpp-ut2.cpp
UT3_SOURCE = testcpp-ut3.cpp
UT4_SOURCE = testcpp-ut4.cpp
UT5_SOURCE = testcpp-ut5.cpp
UT6_SOURCE = testcpp-ut6.cpp

noinst_PROGRAMS = \
  testcpp-ut1 \
  testcpp-ut2 \
  testcpp-ut3 \
  testcpp-ut4 \
  testcpp-ut5 \
  testcpp-ut6

testcpp_ut1_SOURCES = $(UT1_SOURCE)
testcpp_ut2_SOURCES = $(UT2_SOURCE)
testcpp_ut3_SOURCES = $(UT3_SOURCE)
testcpp_ut4_SOURCES = $(UT4_SOURCE)
testcpp_ut5_SOURCES = $(UT5_SOURCE)
testcpp_ut6_SOURCES = $(UT6_SOURCE)

testcpp_ut6_LDADD = \
  $(top_builddir)/src/listeners/libtestcppxmllistener.la

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
  TestStringToOptions.h

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
