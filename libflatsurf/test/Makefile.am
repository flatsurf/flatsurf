check_PROGRAMS = cereal.test quadratic_polynomial.test approximation.test half_edge.test chain.test vector_longlong.test contour_decomposition.test delaunay.test saddle_connections.test vector_exactreal.test permutation.test flat_triangulation_combinatorial.test flat_triangulation.test flow_decomposition.test flat_triangulation_collapsed.test vertex.test edge.test parabolic.test bound.test vector.test

TESTS = $(check_PROGRAMS)

approximation_test_SOURCES = approximation.test.cc main.cc generators/real_generator.hpp
bound_test_SOURCES = bound.test.cc main.cc
cereal_test_SOURCES = cereal.test.cc cereal.helpers.hpp main.cc surfaces.hpp generators/saddle_connections_generator.hpp
chain_test_SOURCES = chain.test.cc main.cc
contour_decomposition_test_SOURCES = contour_decomposition.test.cc main.cc surfaces.hpp generators/vertical_generator.hpp generators/surface_generator.hpp
delaunay_test_SOURCES = delaunay.test.cc main.cc generators/half_edge_generator.hpp
flat_triangulation_test_SOURCES = flat_triangulation.test.cc main.cc surfaces.hpp
flat_triangulation_collapsed_test_SOURCES = flat_triangulation_collapsed.test.cc main.cc surfaces.hpp generators/saddle_connections_generator.hpp
flat_triangulation_combinatorial_test_SOURCES = flat_triangulation_combinatorial.test.cc main.cc surfaces.hpp generators/combinatorial_surface_generator.hpp
flow_decomposition_test_SOURCES = flow_decomposition.test.cc main.cc surfaces.hpp generators/vertical_generator.hpp generators/surface_generator.hpp
half_edge_test_SOURCES = half_edge.test.cc main.cc
edge_test_SOURCES = edge.test.cc main.cc
permutation_test_SOURCES = permutation.test.cc main.cc
quadratic_polynomial_test_SOURCES = quadratic_polynomial.test.cc main.cc
saddle_connections_test_SOURCES = saddle_connections.test.cc main.cc surfaces.hpp
vector_exactreal_test_SOURCES = vector_exactreal.test.cc main.cc
vector_longlong_test_SOURCES = vector_longlong.test.cc main.cc
vertex_test_SOURCES = vertex.test.cc main.cc surfaces.hpp
parabolic_test_SOURCES = parabolic.test.cc main.cc surfaces.hpp
vector_test_SOURCES = vector.test.cc main.cc

# We vendor the header-only library Cereal (serialization with C++ to be able
# to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST = external/cereal/include
# We vendor the single-header catch2 to run our tests
EXTRA_DIST += external/catch2/single_include/catch2/catch.hpp

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -isystem $(srcdir)/external/cereal/include
AM_LDFLAGS = $(builddir)/../src/libflatsurf.la
# we use gmpxx & flint through exact-real's arb/arf wrappers; since we include
# gmpxx.h, we need to link in gmp since it depends on it
AM_LDFLAGS += -lgmpxx -lgmp
# Since e-antic uses FLINT and ANTIC directly in its header files we need to
# link against it as well.
AM_LDFLAGS += -lflint -lantic
