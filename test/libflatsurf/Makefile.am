check_PROGRAMS = saddle_connections flip vector_exactreal saddle_connections_benchmark

TESTS = $(check_PROGRAMS)

saddle_connections_SOURCES = saddle_connections.test.cc main.hpp surfaces.hpp
saddle_connections_benchmark_SOURCES = saddle_connections.benchmark.cc main.hpp surfaces.hpp
flip_SOURCES = flip.test.cc main.hpp surfaces.hpp
vector_exactreal_SOURCES = vector_exactreal.test.cc main.hpp

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../../src/libflatsurf -I $(builddir)/../../src/libflatsurf
AM_LDFLAGS = $(builddir)/../../src/libflatsurf/libflatsurf.la -lgtest -lbenchmark -lrt -lpthread
# we use gmpxx & flint through exact-real's arb/arf wrappers; since we include
# gmpxx.h, we need to link in gmp since it depends on it
AM_LDFLAGS += -lgmpxx -lgmp -lflint

# Run tests serially so we can see the output without having to check the log file explicitly.
# AUTOMAKE_OPTIONS = serial-tests
