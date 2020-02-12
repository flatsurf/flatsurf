noinst_PROGRAMS = benchmark

benchmark_SOURCES = main.cc flat_triangulation_combinatorial.benchmark.cc vertex.benchmark.cc half_edge.benchmark.cc saddle_connections.benchmark.cc chain.benchmark.cc surfaces.hpp

AM_CPPFLAGS = -I $(srcdir)/.. -I $(builddir)/..
AM_LDFLAGS = $(builddir)/../src/libflatsurf.la
# we use gmpxx & flint through exact-real's arb/arf wrappers; since we include
# gmpxx.h, we need to link in gmp since it depends on it
AM_LDFLAGS += -lgmpxx -lgmp -lflint
# Google Benchmark and its dependencies
AM_LDFLAGS += -lbenchmark -lrt -lpthread
