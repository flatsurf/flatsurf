dnl Process this file with autoconf to produce a configure script.

dnl We need at least autoconf 2.69 for this configure.ac to work.
AC_PREREQ([2.69])
AC_INIT([flatsurf], [0.0.1], [eskin@math.uchicago.edu])

dnl Version number of the library c:r:a.
dnl Set to c+1:0:0 on a non-backwards-compatible change.
dnl Set to c+1:0:a+1 on a backwards-compatible change.
dnl Set to c:r+1:a if the interface is unchanged.
AC_SUBST([libflatsurf_version_info], [0:0:0])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.14.1])

dnl Find C++ Compiler
AC_PROG_CXX

dnl Set INSTALL or use install-sh.
AC_PROG_INSTALL
dnl We build our library with libtool.
LT_INIT

AC_CANONICAL_HOST

dnl We run the valgrind test-suite in make check-valgrind
AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_DFLT([drd], [off])
AX_VALGRIND_DFLT([helgrind], [off])
AX_VALGRIND_CHECK()

dnl Check for required libraries
AC_LANG([C++])

dnl We use some C++17 features such as if constexpr
AX_CXX_COMPILE_STDCXX_17()

dnl We provide a Python module powered by cppyy
dnl We provide a Python module powered by cppyy
AC_ARG_WITH([python], AS_HELP_STRING([--without-python], [Do not build Python module]))
AS_IF([test "x$with_python" != "xno"],
      [AM_PATH_PYTHON([3.6], [have_python=yes], [have_python=no])],
      [have_python=no])
AS_IF([test "x$have_python" = "xyes"],
      [],
      [AS_IF([test "x$with_python" = "xyes"], [AC_MSG_ERROR([Python package requested but Python not found])])])
AM_CONDITIONAL([HAVE_PYTHON], [test "x$have_python" = "xyes"])

AC_CHECK_HEADERS([boost/type_traits.hpp], , AC_MSG_ERROR([boost headers not found]))

dnl flatsurf (but not libflatsurf) uses GMP directly in a few places (without gmpxx)
dnl 
AC_CHECK_LIB([gmp], [__gmpq_init], [have_gmp=yes], AC_MSG_ERROR([GMP library not found]))

dnl flatsurf (but not libflatsurf) uses FLINT directly in a few places through e-antic's is_fmpq()
AC_CHECK_LIB([flint], [_fmpz_promote], [have_flint=yes], AC_MSG_ERROR([FLINT library not found]))

# GMPXX does not contain anything that we can check for with AX_CXX_CHECK_LIB
# so we just check for something from the standard library, i.e., that -lgmpxx
# goes through
AX_CXX_CHECK_LIB([gmpxx], [std::runtime_error::what () const], [have_gmpxx=yes], AC_MSG_ERROR([GMP library for C++ not found]), [-lgmp -lmpfr -lflint])

dnl We use Arb for ball arithmetic in our geometry
AC_CHECK_LIB([arb], [arb_init], [have_arb=yes], AC_MSG_ERROR([arb library not found]), [-lgmp -lmpfr -lflint])
AC_CHECK_HEADERS([arb.h], , AC_MSG_ERROR([arb headers not found]))

AC_CHECK_LIB([eantic], [renf_init], [have_eantic=yes], AC_MSG_ERROR([e-antic library not found]), [-lgmp -lmpfr -lflint])
AX_CXX_CHECK_LIB([eanticxx], [eantic::renf_elem_class], [have_eanticxx=yes], AC_MSG_ERROR([E-ANTIC library not found]), [-lgmp -lmpfr -lflint])

dnl flatsurf (but not libflatsurf) uses NTL
AX_CXX_CHECK_LIB([ntl], [NTL::ZZ::zero()], [have_ntl=yes], AC_MSG_ERROR([NTL library not found]), [-lgmp -lmpfr -lflint])
AC_CHECK_HEADERS([NTL/LLL.h NTL/mat_ZZ.h], , AC_MSG_ERROR([NTL headers not found]))

AX_CXX_CHECK_LIB([exactreal], [exactreal::RealNumber::random()], [have_exactreal=yes], AC_MSG_ERROR([exact-real library not found]), [-lgmp -lmpfr -lflint])
AC_CHECK_HEADERS([exact-real/real_number.hpp exact-real/module.hpp], , AC_MSG_ERROR([libexactreal headers not found]))

dnl Our test suite uses googletest and Google's C++ benchmark library.
dnl We fail if they cannot be found but let the user disable all checks explicitly.
AC_ARG_WITH([googletest], AS_HELP_STRING([--without-googletest], [Do not build C++ tests that require googletest/benchmark]))
AS_IF([test "x$with_googletest" != "xno"],
      [
       with_googletest=yes
       AC_CHECK_HEADERS([gtest/gtest.h], , AC_MSG_ERROR([googletest headers for make check not found; run --without-googletest to disable these tests in make check]))
       AC_CHECK_HEADERS([benchmark/benchmark.h], , AC_MSG_ERROR([benchmark headers for make check not found; run --without-googletest to disable these tests in make check]))
      ], [])
AM_CONDITIONAL([HAVE_GOOGLETEST], [test "x$with_googletest" = "xyes"])

dnl Our Python library relies on cppyy. It can be built without since it is a
dnl pure Python package but we cannot test it if cppyy is not present.
AC_ARG_WITH([pytest], AS_HELP_STRING([--without-pytest], [Do not run Python tests]))
AS_IF([test "x$with_pytest" != "xno" && test "x$have_python" = "xyes"],
      [
       with_pytest=yes
       AS_IF([$PYTHON -c 'import pytest'], , AC_MSG_ERROR([pytest for make check not found; run --without-pytest to disable Python tests in make check]))
       AS_IF([$PYTHON -c 'import cppyy'], , AC_MSG_ERROR([cppyy for make check not found; run --without-pytest to disable Python tests in make check]))
      ], [])
AM_CONDITIONAL([HAVE_PYTEST], [test "x$with_pytest" = "xyes"])

AC_CONFIG_HEADERS([src/libflatsurf/flatsurf/config.h])
AC_CONFIG_FILES([Makefile src/Makefile src/libflatsurf/Makefile src/pyflatsurf/Makefile src/flatsurf/Makefile test/Makefile test/flatsurf/Makefile test/libflatsurf/Makefile test/pyflatsurf/Makefile])
AC_OUTPUT
