check_PROGRAMS =                        \
  approximation.test                    \
  bound.test                            \
  cereal.test                           \
  chain.test                            \
  contour_decomposition.test            \
  deformation.test                      \
  edge.test                             \
  flat_triangulation.test               \
  flat_triangulation_collapsed.test     \
  flat_triangulation_combinatorial.test \
  flow_decomposition.test               \
  half_edge.test                        \
  interval_exchange_tranformation.test  \
  parabolic.test                        \
  path.test                             \
  permutation.test                      \
  quadratic_polynomial.test             \
  saddle_connections.test               \
  saddle_connection.test                \
  vector.test                           \
  vector_exactreal.test                 \
  vertex.test

TESTS = $(check_PROGRAMS)

approximation_test_SOURCES = approximation.test.cc main.cc
bound_test_SOURCES = bound.test.cc main.cc
cereal_test_SOURCES = cereal.test.cc cereal.helpers.hpp main.cc
chain_test_SOURCES = chain.test.cc main.cc
contour_decomposition_test_SOURCES = contour_decomposition.test.cc main.cc
deformation_test_SOURCES = deformation.test.cc main.cc
flat_triangulation_test_SOURCES = flat_triangulation.test.cc main.cc
flat_triangulation_collapsed_test_SOURCES = flat_triangulation_collapsed.test.cc main.cc
flat_triangulation_combinatorial_test_SOURCES = flat_triangulation_combinatorial.test.cc main.cc
flow_decomposition_test_SOURCES = flow_decomposition.test.cc main.cc
half_edge_test_SOURCES = half_edge.test.cc main.cc
interval_exchange_tranformation_test_SOURCES = interval_exchange_transformation.test.cc main.cc
edge_test_SOURCES = edge.test.cc main.cc
path_test_SOURCES = path.test.cc main.cc
permutation_test_SOURCES = permutation.test.cc main.cc
quadratic_polynomial_test_SOURCES = quadratic_polynomial.test.cc main.cc
saddle_connections_test_SOURCES = saddle_connections.test.cc main.cc
saddle_connection_test_SOURCES = saddle_connection.test.cc main.cc
vector_exactreal_test_SOURCES = vector_exactreal.test.cc main.cc
vertex_test_SOURCES = vertex.test.cc main.cc
parabolic_test_SOURCES = parabolic.test.cc main.cc
vector_test_SOURCES = vector.test.cc main.cc

noinst_HEADERS =                                 \
  surfaces.hpp                                   \
  generators/real_generator.hpp                  \
  generators/surface_generator.hpp               \
  generators/vertical_generator.hpp              \
  generators/half_edge_generator.hpp             \
  generators/combinatorial_surface_generator.hpp \
  generators/saddle_connections_generator.hpp

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
# Since e-antic uses ANTIC directly in its header files we need to
# link against it as well.
AM_LDFLAGS += -lantic
