noinst_PROGRAMS = benchmark

benchmark_SOURCES = main.cc vector.benchmark.cc flat_triangulation_combinatorial.benchmark.cc vertex.benchmark.cc half_edge.benchmark.cc saddle_connection.benchmark.cc saddle_connections.benchmark.cc chain.benchmark.cc flat_triangulation_collapsed.benchmark.cc flat_triangulation.benchmark.cc path.benchmark.cc ../test/surfaces.hpp

AM_CPPFLAGS = -I $(srcdir)/.. -I $(builddir)/..
LDADD = $(builddir)/../src/libflatsurf.la
if HAVE_ANTIC
# Since e-antic uses ANTIC directly in its header files we need to
# link against it as well.
LDADD += -lantic
endif
# Google Benchmark and its dependencies
LDADD += -lbenchmark -lpthread

# We use e-antic (C++ interface) directly in our benchmarks
LDADD += -leanticxx
