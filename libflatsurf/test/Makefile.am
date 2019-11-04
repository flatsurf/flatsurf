check_PROGRAMS = length_along_triangulation vector_longlong interval_exchange_transformation delaunay saddle_connections vector_exactreal cereal permutation flat_triangulation_combinatorial flat_triangulation

TESTS = $(check_PROGRAMS)

saddle_connections_SOURCES = saddle_connections.test.cc main.cc surfaces.hpp
vector_exactreal_SOURCES = vector_exactreal.test.cc main.cc
delaunay_SOURCES = delaunay.test.cc main.cc
interval_exchange_transformation_SOURCES = interval_exchange_transformation.test.cc main.cc surfaces.hpp
vector_longlong_SOURCES = vector_longlong.test.cc main.cc
length_along_triangulation_SOURCES = length_along_triangulation.test.cc main.cc surfaces.hpp
cereal_SOURCES = cereal.test.cc cereal.helpers.hpp main.cc surfaces.hpp
permutation_SOURCES = permutation.test.cc main.cc
flat_triangulation_combinatorial_SOURCES = flat_triangulation_combinatorial.test.cc main.cc surfaces.hpp
flat_triangulation_SOURCES = flat_triangulation.test.cc main.cc surfaces.hpp

# We vendor the header-only library Cereal (serialization with C++ to be able
# to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST = external/cereal/include
# We vendor the single-header catch2 to run our tests
EXTRA_DIST += external/catch2/single_include/catch2/catch.hpp

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../src -I $(builddir)/../src -isystem $(srcdir)/external/cereal/include -isystem $(srcdir)/external/catch2/single_include/catch2
AM_LDFLAGS = $(builddir)/../src/libflatsurf.la
# we use gmpxx & flint through exact-real's arb/arf wrappers; since we include
# gmpxx.h, we need to link in gmp since it depends on it
AM_LDFLAGS += -lgmpxx -lgmp -lflint
