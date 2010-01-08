
AM_CPPFLAGS = \
  -I$(top_builddir)/include \
  $(LTDLINCL)

lib_LTLIBRARIES = libtestngpprunner.la 

libtestngpprunner_la_LDFLAGS = \
  -no-undefined \
  $(top_builddir)/src/libtestngpp.la \
  $(LIBLTDL)

nodist_libtestngpprunner_la_SOURCES = \
  TestSuiteRunner.cpp \
  TestCaseSandboxResultReporter.cpp \
  TestCaseSandboxResultDecoder.cpp \
  SimpleTestCaseRunner.cpp \
  Sandbox.cpp \
  SandboxProcessor.cpp \
  TestCaseSandboxHandler.cpp \
  TestCaseSandbox.cpp \
  PipeReadableChannel.cpp \
  PipeWrittableChannel.cpp  \
  TestFixtureSandboxRunner.cpp \
  TestFixtureRunnerWrapper.cpp \
  LTTestSuiteLoader.cpp \
  SimpleTestResultDispatcher.cpp \
  SimpleTestResultReporter.cpp \
  SimpleTestCaseResultReporter.cpp \
  SimpleTestSuiteResultReporter.cpp \
  TestFixtureRunnerFactory.cpp \
  SimpleTestFixtureRunner.cpp \
  LTTestListenerLoader.cpp \
  WildcardTestFilter.cpp \
  OrCompositeTestFilter.cpp \
  GeneralTestFilter.cpp \
  TestFilterFactory.cpp \
  PosixFNMatcher.cpp \
  TestRunner.cpp

bin_PROGRAMS = testngpp-runner

testngpp_runner_LDADD = \
  $(top_builddir)/src/runner/libtestngpprunner.la 

testngpp_runner_LDFLAGS = \
  -export-dynamic

testngpp_runner_SOURCES = \
  TestRunnerMain.cpp
