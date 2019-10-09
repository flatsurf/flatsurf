dnl Process this file with autoconf to produce a configure script.

dnl We need at least autoconf 2.69 for this configure.ac to work.
AC_PREREQ([2.69])
AC_INIT([flatsurf-polygon], [0.0.1], [julian.rueth@fsfe.org])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.14.1])

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

dnl flatsurf-polygon (but not libflatsurf) uses GMP directly in a few places (without gmpxx)
dnl 
AC_CHECK_LIB([gmp], [__gmpq_init], [have_gmp=yes], AC_MSG_ERROR([GMP library not found]))

dnl flatsurf-polygon (but not libflatsurf) uses FLINT directly in a few places through e-antic's is_fmpq()
AC_CHECK_LIB([flint], [_fmpz_promote], [have_flint=yes], AC_MSG_ERROR([FLINT library not found]))

dnl flatsurf-polygon (but not libflatsurf) uses NTL
AX_CXX_CHECK_LIB([ntl], [NTL::ZZ::zero()], [have_ntl=yes], AC_MSG_ERROR([NTL library not found]), [-lgmp -lmpfr -lflint])
AC_CHECK_HEADERS([NTL/LLL.h NTL/mat_ZZ.h], , AC_MSG_ERROR([NTL headers not found]))

dnl flatsurf-polygon (but not libflatsurf yet) use Eigen for some Linear Algebra
AC_CHECK_HEADERS([eigen3/Eigen/Dense eigen/Eigen/Dense], [have_eigen=yes; break;])

AS_IF([test "x$have_eigen" != "xyes"], [AC_MSG_ERROR([Eigen headers not found])])

AC_CHECK_LIB([eantic], [renf_init], [have_eantic=yes], AC_MSG_ERROR([e-antic library not found]), [-lgmp -lmpfr -lflint])
AX_CXX_CHECK_LIB([eanticxx], [eantic::renf_elem_class], [have_eanticxx=yes], AC_MSG_ERROR([E-ANTIC library not found]), [-lgmp -lmpfr -lflint])

AX_CXX_CHECK_LIB([exactreal], [exactreal::RealNumber::arb (long prec = 0) const], [have_exactreal=yes], AC_MSG_ERROR([exact-real library not found]), [-lgmp -lmpfr -lflint])
AC_CHECK_HEADERS([exact-real/real_number.hpp exact-real/module.hpp], , AC_MSG_ERROR([libexactreal headers not found]))

# intervalxt does not contain anything that we can check for with AX_CXX_CHECK_LIB
# so we just check for something from the standard library, i.e., that -lintervalxt
# goes through
AX_CXX_CHECK_LIB([intervalxt], [std::runtime_error::what () const], [have_intervalxt=yes], AC_MSG_ERROR([intervalxt library not found]), [-lgmp -lmpfr -lflint])
AC_CHECK_HEADERS([intervalxt/intervalxt.hpp], , AC_MSG_ERROR([libintervalxt headers not found]))


AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])

AC_OUTPUT
