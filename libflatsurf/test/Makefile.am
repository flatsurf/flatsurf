check_PROGRAMS = main

TESTS = libflatsurf.test.sh

main_SOURCES = main.cc approximation.test.cc bound.test.cc chain.test.cc contour_component.test.cc contour_decomposition.test.cc deformation.test.cc edge.test.cc equivalence.test.cc equivalence_class.test.cc flat_triangulation.test.cc flat_triangulation_collapsed.test.cc flat_triangulation_combinatorial.test.cc flow_decomposition.test.cc half_edge.test.cc interval_exchange_transformation.test.cc path.test.cc permutation.test.cc point.test.cc quadratic_polynomial.test.cc saddle_connections.test.cc saddle_connection.test.cc vector_longlong.test.cc vector.test.cc vertex.test.cc vertical.test.cc

noinst_HEADERS =                                 \
  surfaces.hpp                                   \
  cereal.helpers.hpp                             \
  generators/point_generator.hpp                 \
  generators/combinatorial_surface_generator.hpp \
  generators/deformation_generator.hpp           \
  generators/equivalence_generator.hpp           \
  generators/half_edge_generator.hpp             \
  generators/real_generator.hpp                  \
  generators/saddle_connection_generator.hpp     \
  generators/surface_generator.hpp               \
  generators/vertical_generator.hpp

# We vendor the header-only library Cereal (serialization with C++ to be able
# to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST = external/cereal/include
# We vendor the single-header catch2 to run our tests
EXTRA_DIST += external/catch2/single_include/catch2/catch.hpp

EXTRA_DIST += libflatsurf.test.sh

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -isystem $(srcdir)/external/cereal/include
AM_LDFLAGS = $(builddir)/../src/libflatsurf.la
if HAVE_ANTIC
# Since e-antic uses ANTIC directly in its header files we need to
# link against it as well.
AM_LDFLAGS += -lantic
endif
