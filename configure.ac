dnl Process this file with autoconf to produce a configure script.

dnl We need at least autoconf 2.69 for this configure.ac to work.
AC_PREREQ([2.69])
AC_INIT([polygon], [0.0.1], [eskin@math.uchicago.edu])

dnl Version number of the library c:r:a.
dnl Set to c+1:0:0 on a non-backwards-compatible change.
dnl Set to c+1:0:a+1 on a backwards-compatible change.
dnl Set to c:r+1:a if the interface is unchanged.
AC_SUBST([libpolygon_version_info], [0:0:0])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.14.1])

dnl Find C++ Compiler
AC_PROG_CXX

dnl Set INSTALL or use install-sh.
AC_PROG_INSTALL
dnl We build our library with libtool.
LT_INIT

AC_CANONICAL_HOST

dnl Check for required libraries
AC_LANG([C++])

dnl We use GMP for big rationals (which links against libm)
AC_CHECK_LIB([m], [log], , AC_MSG_ERROR([libm not found]))
AC_CHECK_LIB([gmp], [__gmpq_init], , AC_MSG_ERROR([GMP library not found]))
AC_CHECK_LIB([gmpxx], [__gmpq_get_str], , AC_MSG_ERROR([GMP library (C++) not found]))
AC_CHECK_HEADERS([gmpxx.h], [], AC_MSG_ERROR([gmpxx header not found (gmp needs to be compiled with c++ support (--enable-cxx))]))

dnl We use NTL for integer matrices and LLL (which somehow needs phtread linked in)
AC_CHECK_LIB([pthread], [pthread_create], , AC_MSG_ERROR([pthread library not fonud]))
AX_CXX_CHECK_LIB([ntl], [NTL::ZZ::zero()], , AC_MSG_ERROR([NTL library not found]))
dnl NTL needs C++11
AX_CXX_COMPILE_STDCXX(11)
AC_CHECK_HEADERS([NTL/LLL.h NTL/mat_ZZ.h], , AC_MSG_ERROR([NTL headers not found]))

dnl There is a code path that uses MPFR but it is not enabled currently
dnl AC_CHECK_LIB([mpfr], [mpfr_init], , AC_MSG_ERROR([MPFR library not found]))

dnl We use boost (e.g., to work with MPFR)
AC_CHECK_HEADERS([boost/numeric/ublas/matrix.hpp], , AC_MSG_ERROR([Boost BLAS headers not found]))

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
