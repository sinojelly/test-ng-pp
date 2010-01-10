
AC_INIT([TestNG++], [0.3], [arthur.ii.yuan@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_LIBTOOL
AC_PROG_CXX
AM_PATH_PYTHON([2.4],,[:])

AC_SEARCH_LIBS([lt_dlopenext],[ltdl])
AM_CONDITIONAL([TESTNGPP_USE_LTDL], [test "$ac_cv_search_lt_dlopenext" != no])

AC_SEARCH_LIBS([dlopen],[dl])
AM_CONDITIONAL([TESTNGPP_USE_DLOPEN], [test "$ac_cv_search_dlopen" != no])

AC_CONFIG_SUBDIRS([3rdparty/mockcpp])
AC_CONFIG_FILES([
  Makefile
  scripts/Makefile
  src/Makefile
  src/runner/Makefile
  src/listeners/Makefile
  libs/Makefile
  ut/Makefile
  samples/Makefile
])
AC_OUTPUT
