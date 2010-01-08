
TESTNGPP_INCLUDE_PATH = $(top_builddir)/include/testngpp

AM_CPPFLAGS = \
  -I$(top_builddir)/include

AM_LDFLAGS = \
  -module \
  -rpath /dev/null \
  -no-undefined \
  $(top_builddir)/src/libtestngpp.la

noinst_LTLIBRARIES = libtestngppstdoutlistener.la libtestngppxmllistener.la

libtestngppstdoutlistener_la_SOURCES= StdoutTestListener.cpp
libtestngppxmllistener_la_SOURCES= XMLTestListener.cpp XMLBuilder.cpp
