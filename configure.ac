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
AM_PATH_PYTHON()

AC_CHECK_HEADERS([boost/type_traits.hpp], , AC_MSG_ERROR([boost headers not found]))

dnl flatsurf uses GMP/MPFR directly in a few places
# AC_CHECK_LIB([gmp], [__gmpq_init], , AC_MSG_ERROR([GMP library not found]))
# AC_CHECK_LIB([mpfr], [mpfr_init], , AC_MSG_ERROR([MPFR library not found]))
# GMPXX does not contain anything that we can check for with AX_CXX_CHECK_LIB
# so we just check for something from the standard library, i.e., that -lgmpxx
# goes through
AX_CXX_CHECK_LIB([gmpxx], [std::runtime_error::what () const], , AC_MSG_ERROR([GMP library for C++ not found]), [-lgmp -lmpfr -lflint])

dnl We use Arb for ball arithmetic in our geometry
AC_CHECK_LIB([arb], [arb_init], , AC_MSG_ERROR([arb library not found]), [-lgmp -lmpfr -lflint])
AC_CHECK_HEADERS([arb.h], , AC_MSG_ERROR([arb headers not found]))

AC_CHECK_LIB([eantic], [renf_init], , AC_MSG_ERROR([e-antic library not found]), [-lgmp -lmpfr -lflint])
AX_CXX_CHECK_LIB([eanticxx], [eantic::renf_elem_class], , AC_MSG_ERROR([E-ANTIC library not found]), [-lgmp -lmpfr -lflint])

dnl flatsurf uses NTL
AX_CXX_CHECK_LIB([ntl], [NTL::ZZ::zero()], , AC_MSG_ERROR([NTL library not found]), [-lgmp -lmpfr -lflint])
AC_CHECK_HEADERS([NTL/LLL.h NTL/mat_ZZ.h], , AC_MSG_ERROR([NTL headers not found]))

AX_CXX_CHECK_LIB([exactreal], [exactreal::RealNumber::random()], , AC_MSG_ERROR([exact-real library not found]), [-lgmp -lmpfr -lflint])
AC_CHECK_HEADERS([exact-real/real_number.hpp exact-real/module.hpp], , AC_MSG_ERROR([libexactreal headers not found]))

AC_CONFIG_HEADERS([src/libflatsurf/flatsurf/config.h])
AC_CONFIG_FILES([Makefile src/Makefile src/libflatsurf/Makefile src/libpyflatsurf/Makefile src/flatsurf/Makefile test/Makefile test/flatsurf/Makefile test/libflatsurf/Makefile test/libpyflatsurf/Makefile])
AC_OUTPUT
