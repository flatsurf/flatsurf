check_PROGRAMS = saddle_connections flip vector_exactreal

TESTS = $(check_PROGRAMS)

saddle_connections_SOURCES = saddle_connections.test.cc main.hpp surfaces.hpp
flip_SOURCES = flip.test.cc main.hpp surfaces.hpp
vector_exactreal_SOURCES = vector_exactreal.test.cc main.hpp

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../../src/libflatsurf -I $(builddir)/../../src/libflatsurf
AM_LDFLAGS = $(builddir)/../../src/libflatsurf/libflatsurf.la -lgtest -lbenchmark -lrt -lpthread
# we use gmpxx & flint through exact-real's arb/arf wrappers
AM_LDFLAGS += -lgmpxx -lflint

# Run tests serially so we can see the output without having to check the log file explicitly.
# AUTOMAKE_OPTIONS = serial-tests
