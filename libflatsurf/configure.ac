dnl Process this file with autoconf to produce a configure script.

dnl We need at least autoconf 2.69 for this configure.ac to work.
AC_PREREQ([2.69])
AC_INIT([libflatsurf], [3.15.2], [julian.rueth@fsfe.org])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([.])
dnl Request tests to run serially since parallel takes care of actually running tests in parallel in libflatsurf.test.sh
AM_INIT_AUTOMAKE([1.14.1 tar-pax subdir-objects foreign serial-tests])

dnl Version number of the library c:r:a.
dnl Set to c+1:0:0 on a non-backwards-compatible change.
dnl Set to c+1:0:a+1 on a backwards-compatible change.
dnl Set to c:r+1:a if the interface is unchanged.
AC_SUBST([libflatsurf_version_info], [21:2:14])

AC_CANONICAL_HOST

dnl Find C++ Compiler
AC_PROG_CXX

dnl Set INSTALL or use install-sh.
AC_PROG_INSTALL

dnl We build our library with libtool.
LT_INIT

dnl We run the valgrind test-suite in make check-valgrind
AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_DFLT([drd], [off])
AX_VALGRIND_DFLT([helgrind], [off])
AX_VALGRIND_CHECK()

dnl Check for required libraries
AC_LANG([C++])

dnl We use some C++17 features such as if constexpr
AX_CXX_COMPILE_STDCXX(17)

AC_CHECK_HEADERS([boost/type_traits.hpp], , AC_MSG_ERROR([boost headers not found]))

AC_CHECK_HEADERS([fmt/format.h], , AC_MSG_ERROR([fmt headers not found]))
AX_CXX_CHECK_LIB([fmt], [fmt::v6::getpagesize()], , [
  AX_CXX_CHECK_LIB([fmt], [fmt::v7::getpagesize()], , [
    AX_CXX_CHECK_LIB([fmt], [fmt::v8::report_system_error(int code=0, const char* message=nullptr)], , [
      AX_CXX_CHECK_LIB([fmt], [fmt::v9::report_system_error(int code=0, const char* message=nullptr)], , [
        AX_CXX_CHECK_LIB([fmt], [fmt::v10::report_system_error(int code=0, const char* message=nullptr)], , [AC_MSG_ERROR([fmt library not found])], [-lfmt])
      ], [-lfmt])
    ], [-lfmt])
  ], [-lfmt])
], [-lfmt])

AC_CHECK_HEADER(gmp.h, , [AC_MSG_ERROR([GMP header not found])])
AC_CHECK_LIB(gmp, __gmpz_add, [], [AC_MSG_ERROR([libgmp not found])])

AC_CHECK_HEADERS(flint/flint.h flint/fmpz.h flint/fmpq.h, , [AC_MSG_ERROR([FLINT headers not found])])
AC_CHECK_LIB(flint, fmpz_init, [], [AC_MSG_ERROR([libflint not found])])

# GMPXX does not contain anything that we can check for with AX_CXX_CHECK_LIB
# so we just check for something from the standard library, i.e., that -lgmpxx
# goes through
AX_CXX_CHECK_LIB([gmpxx], [std::runtime_error::what () const], , AC_MSG_ERROR([GMP library for C++ not found]), [-lmpfr])

AC_CHECK_HEADERS([gmpxxll/mpz_class.hpp], [ ], [AC_MSG_ERROR([gmpxxll headers not found])])

dnl We use Arb for ball arithmetic in our geometry
AC_CHECK_HEADERS([flint/arb.h], [
  dnl We are building against FLINT 3 (which ships Arb & ANTIC)
  have_arb=no
  have_antic=no
  AC_CHECK_LIB([flint], [arb_load_str], , AC_MSG_ERROR([FLINT library >=3 not found]), [-lgmp -lmpfr -lflint])
], [
  dnl We are building against FLINT 2 (which does not ship Arb yet.)
  AC_CHECK_HEADERS([arb.h], , AC_MSG_ERROR([Arb headers not found]))
  AC_CHECK_LIB([arb], [arb_load_str], [have_arb=yes], AC_MSG_ERROR([Arb library >=2.17.0 not found]), [-lgmp -lmpfr -lflint])

  AC_CHECK_LIB([antic], [nf_init], [have_antic=yes], AC_MSG_ERROR([ANTIC library not found]), [-lmpfr])
])

AM_CONDITIONAL([HAVE_ARB], [test "x$have_arb" = "xyes"])
AM_CONDITIONAL([HAVE_ANTIC], [test "x$have_antic" = "xyes"])

AC_CHECK_LIB([eantic], [renf_init], [have_eantic=yes], AC_MSG_ERROR([e-antic library not found]), [-lmpfr])
AX_CXX_CHECK_LIB([eanticxx], [eantic::renf_elem_class], [have_eanticxx=yes], AC_MSG_ERROR([e-antic library not found]), [-lmpfr])


AX_CXX_CHECK_LIB([exactreal], [exactreal::RealNumber::arb (long prec = 0) const], [have_exactreal=yes], AC_MSG_ERROR([exact-real library not found]), [-lmpfr])
AC_CHECK_HEADERS([exact-real/real_number.hpp exact-real/module.hpp], , AC_MSG_ERROR([libexactreal headers not found]))

# intervalxt does not contain anything that we can check for with AX_CXX_CHECK_LIB
# so we just check for something from the standard library, i.e., that -lintervalxt
# goes through
AX_CXX_CHECK_LIB([intervalxt], [std::runtime_error::what () const], [have_intervalxt=yes], AC_MSG_ERROR([intervalxt library not found]), [-lmpfr])
AC_CHECK_HEADERS([intervalxt/intervalxt.hpp], , AC_MSG_ERROR([libintervalxt headers not found]))

dnl Our benchmarks use Google's C++ benchmark library.
dnl We fail if they cannot be found but let the user disable it explicitly.
AC_ARG_WITH([benchmark], AS_HELP_STRING([--without-benchmark], [Do not build C++ benchmarks that require google/benchmark]))
AS_IF([test "x$with_benchmark" != "xno"],
      [
       with_benchmark=yes
       AC_CHECK_HEADERS([benchmark/benchmark.h], , AC_MSG_ERROR([benchmark headers not found; run --without-benchmark to disable building of benchmark/]))
      ], [])
AM_CONDITIONAL([HAVE_BENCHMARK], [test "x$with_benchmark" = "xyes"])

AC_CONFIG_HEADERS([flatsurf/config.h])
AC_CONFIG_FILES([Makefile src/Makefile test/Makefile benchmark/Makefile])

AC_OUTPUT
