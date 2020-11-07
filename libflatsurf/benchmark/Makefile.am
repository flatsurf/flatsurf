noinst_PROGRAMS = benchmark

benchmark_SOURCES = main.cc vector.benchmark.cc flat_triangulation_combinatorial.benchmark.cc vertex.benchmark.cc half_edge.benchmark.cc saddle_connection.benchmark.cc saddle_connections.benchmark.cc chain.benchmark.cc flat_triangulation_collapsed.benchmark.cc flat_triangulation.benchmark.cc path.benchmark.cc ../test/surfaces.hpp

AM_CPPFLAGS = -I $(srcdir)/.. -I $(builddir)/..
AM_LDFLAGS = $(builddir)/../src/libflatsurf.la
# we use gmpxx & flint through exact-real's arb/arf wrappers; since we include
# gmpxx.h, we need to link in gmp since it depends on it
AM_LDFLAGS += -lgmpxx -lgmp
# Since e-antic uses FLINT and ANTIC directly in its header files we need to
# link against it as well.
AM_LDFLAGS += -lflint -lantic
# Google Benchmark and its dependencies
AM_LDFLAGS += -lbenchmark -lrt -lpthread
