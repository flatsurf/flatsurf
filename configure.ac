dnl Process this file with autoconf to produce a configure script.

dnl We need at least autoconf 2.69 for this configure.ac to work.
AC_PREREQ([2.69])
AC_INIT([polygon], [0.0.1], [eskin@math.uchicago.edu])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.14.1])

dnl Find C++ Compiler
AC_PROG_CXX

dnl Set INSTALL or use install-sh.
AC_PROG_INSTALL

AC_CANONICAL_HOST

dnl Check for required libraries
AC_LANG([C++])

dnl We use GMP for big rationals
AC_CHECK_LIB([gmp], [__gmpq_init], , AC_MSG_ERROR([GMP library not found]))
AC_CHECK_LIB([gmpxx], [__gmpq_get_str], , AC_MSG_ERROR([GMP library (C++) not found]))
AC_CHECK_HEADERS([gmpxx.h], [], AC_MSG_ERROR([gmpxx header not found (gmp needs to be compiled with c++ support (--enable-cxx))]))

dnl We use NTL for integer matrices and LLL (which links against phtread)
AC_CHECK_LIB([pthread], [pthread_create])
AX_CXX_CHECK_LIB([ntl], [NTL::ZZ::zero()], , AC_MSG_ERROR([NTL library not found]))
AC_CHECK_HEADERS([NTL/mat_ZZ.h NTL/LLL.h], , AC_MSG_ERROR([NTL headers not found]))

dnl We use MPFR through Boost
AC_CHECK_LIB([mpfr], [mpfr_init], , AC_MSG_ERROR([MPFR library not found]))
AC_CHECK_HEADERS([boost/multiprecision/mpfr.hpp], , AC_MSG_ERROR([Boost MPFR headers not found]))

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
