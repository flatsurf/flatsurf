check_PROGRAMS = saddle_connections flip

TESTS = $(check_PROGRAMS)

saddle_connections_SOURCES = saddle_connections.test.cc main.hpp surfaces.hpp
flip_SOURCES = flip.test.cc main.hpp surfaces.hpp

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../../src/libflatsurf -I $(builddir)/../../src/libflatsurf
AM_LDFLAGS = $(builddir)/../../src/libflatsurf/libflatsurf.la -lgtest -lbenchmark -lrt -lpthread
# arb.hpp & arf.hpp use gmpxx; it appears that we need to link gmp in explicitly as well to make gmpxx happy
AM_LDFLAGS += -lgmpxx -lgmp
# arb.hpp & arf.hpp use flint
AM_LDFLAGS += -lflint

# Run tests serially so we can see the output without having to check the log file explicitly.
# AUTOMAKE_OPTIONS = serial-tests
