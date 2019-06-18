lib_LTLIBRARIES = libflatsurf.la

libflatsurf_la_SOURCES =                                      \
	ccw.cc                                                      \
	delaunay_triangulation.cc                                   \
	edge.cc                                                     \
	flat_triangulation.cc                                       \
	flat_triangulation_combinatorial.cc                         \
	half_edge.cc                                                \
	half_edge_map.cc                                            \
	interval_exchange_transformation.cc                         \
	length_along_triangulation.cc                               \
	permutation.cc                                              \
	saddle_connections.cc                                       \
	vertex.cc                                                   \
	vector/vector.cc                                            \
	vector/vector_along_triangulation.cc

nobase_include_HEADERS =                                      \
	flatsurf/ccw.hpp                                            \
	flatsurf/component.hpp                                      \
	flatsurf/cppyy.hpp                                          \
	flatsurf/delaunay_triangulation.hpp                         \
	flatsurf/edge.hpp                                           \
	flatsurf/flatsurf.hpp                                       \
	flatsurf/flat_triangulation.hpp                             \
	flatsurf/flat_triangulation_combinatorial.hpp               \
	flatsurf/flow_decomposition.hpp                             \
	flatsurf/forward.hpp                                        \
	flatsurf/half_edge.hpp                                      \
	flatsurf/half_edge_map.hpp                                  \
	flatsurf/interval_exchange_transformation.hpp               \
	flatsurf/length_along_triangulation.hpp                     \
	flatsurf/orientation.hpp                                    \
	flatsurf/permutation.hpp                                    \
	flatsurf/saddle_connections.hpp                             \
	flatsurf/saddle_connection.hpp                              \
	flatsurf/vector.hpp                                         \
	flatsurf/vector_along_triangulation.hpp                     \
	flatsurf/vertex.hpp

nobase_include_HEADERS +=                                     \
	flatsurf/detail/vector_base.hpp                             \
	flatsurf/detail/vector_exact.hpp                            \
	flatsurf/detail/vector_with_error.hpp                       \
	flatsurf/external/spimpl/spimpl.h

noinst_HEADERS =                                             \
	saddle_connection.ipp                                      \
	util/type_traits.ipp                                       \
	util/assert.ipp                                            \
	util/as_vector.ipp                                         \
	util/false.ipp                                             \
	vector/algorithm/exact.ipp                                 \
	vector/algorithm/exact.extension.ipp                       \
	vector/algorithm/extension.ipp                             \
	vector/algorithm/base.ipp                                  \
	vector/algorithm/base.extension.ipp                        \
	vector/algorithm/with_error.ipp                            \
	vector/algorithm/with_error.extension.ipp                  \
	vector/storage/forward.ipp                                 \
	vector/storage/cartesian.ipp

libflatsurf_la_LDFLAGS = -version-info $(libflatsurf_version_info)
# some of our vectors use arb directly and through exact-real's arb wrappers
libflatsurf_la_LDFLAGS += -larb
# we use exact-real in our vectors
libflatsurf_la_LDFLAGS += -lexactreal
# we use e-antic in our vectors
libflatsurf_la_LDFLAGS += -leanticxx -leantic
# we build IETs with intervalxt
libflatsurf_la_LDFLAGS += -lintervalxt

$(builddir)/flatsurf/flatsurf.hpp: $(srcdir)/flatsurf/flatsurf.hpp.in Makefile
	mkdir -p $(builddir)/flatsurf
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate flatsurf.hpp, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Generate flatsurf.hpp before performing any compilation
BUILT_SOURCES = flatsurf/flatsurf.hpp
# Distribute flatsurf.hpp.in as part of the tarball, but not flatsurf.hpp
EXTRA_DIST = flatsurf/flatsurf.hpp.in
nodist_include_HEADERS = flatsurf/flatsurf.hpp

CLEANFILES = flatsurf/flatsurf.hpp
