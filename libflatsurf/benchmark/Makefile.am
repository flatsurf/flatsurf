noinst_PROGRAMS = benchmark

benchmark_SOURCES = main.cc vector.benchmark.cc flat_triangulation_combinatorial.benchmark.cc vertex.benchmark.cc half_edge.benchmark.cc saddle_connection.benchmark.cc saddle_connections.benchmark.cc chain.benchmark.cc flat_triangulation_collapsed.benchmark.cc flat_triangulation.benchmark.cc path.benchmark.cc ../test/surfaces.hpp

AM_CPPFLAGS = -I $(srcdir)/.. -I $(builddir)/..
LDADD = $(builddir)/../src/libflatsurf.la
# Google Benchmark and its dependencies
LDADD += -lbenchmark -lpthread
