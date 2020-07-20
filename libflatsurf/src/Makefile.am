lib_LTLIBRARIES = libflatsurf.la

libflatsurf_la_SOURCES =                                            \
	approximation.cc                                            \
	bound.cc                                                    \
	ccw.cc                                                      \
	chain.cc                                                    \
	chain_vector.cc                                             \
	chain_iterator.cc                                           \
	collapsed_half_edge.cc                                      \
	contour_component.cc                                        \
	contour_component_state.cc                                  \
	contour_connection.cc                                       \
	contour_decomposition.cc                                    \
	contour_decomposition_state.cc                              \
	delaunay_triangulation.cc                                   \
	edge.cc                                                     \
	edge_set_iterator.cc                                        \
	edge_set.cc                                                 \
	half_edge_set.cc                                            \
	half_edge_set_iterator.cc                                   \
	flat_triangulation.cc                                       \
	flat_triangulation_collapsed.cc                             \
	flat_triangulation_combinatorial.cc                         \
	flow_component.cc                                           \
	flow_component_state.cc                                     \
	flow_connection.cc                                          \
	flow_decomposition.cc                                       \
	flow_decomposition_state.cc                                 \
	flow_triangulation.cc                                       \
	half_edge.cc                                                \
	indexed_set.cc                                              \
	indexed_set_iterator.cc                                     \
	interval_exchange_transformation.cc                         \
	lengths.cc                                                  \
	path.cc                                                     \
	path_iterator.cc                                            \
	permutation.cc                                              \
	quadratic_polynomial.cc                                     \
	saddle_connection.cc                                        \
	saddle_connections.cc                                       \
	saddle_connections_iterator.cc                              \
	tracked.cc                                                  \
	vector/vector.cc                                            \
	vertex.cc                                                   \
	vertical.cc

nobase_pkginclude_HEADERS =                                         \
	../flatsurf/bound.hpp                                       \
	../flatsurf/ccw.hpp                                         \
	../flatsurf/cereal.hpp                                      \
	../flatsurf/chain.hpp                                       \
	../flatsurf/chain_iterator.hpp                              \
	../flatsurf/contour_component.hpp                           \
	../flatsurf/contour_connection.hpp                          \
	../flatsurf/contour_decomposition.hpp                       \
	../flatsurf/copyable.hpp                                    \
	../flatsurf/cppyy.hpp                                       \
	../flatsurf/decomposition_step.hpp                          \
	../flatsurf/delaunay_triangulation.hpp                      \
	../flatsurf/edge.hpp                                        \
	../flatsurf/edge_map.hpp                                    \
	../flatsurf/edge_set.hpp                                    \
	../flatsurf/edge_set_iterator.hpp                           \
	../flatsurf/flat_triangulation.hpp                          \
	../flatsurf/flat_triangulation_collapsed.hpp                \
	../flatsurf/flat_triangulation_combinatorial.hpp            \
	../flatsurf/flatsurf.hpp                                    \
	../flatsurf/flow_component.hpp                              \
	../flatsurf/flow_connection.hpp                             \
	../flatsurf/flow_decomposition.hpp                          \
	../flatsurf/flow_triangulation.hpp                          \
	../flatsurf/fmt.hpp                                         \
	../flatsurf/forward.hpp                                     \
	../flatsurf/half_edge.hpp                                   \
	../flatsurf/half_edge_map.hpp                               \
	../flatsurf/half_edge_set.hpp                               \
	../flatsurf/half_edge_set_iterator.hpp                      \
	../flatsurf/interval_exchange_transformation.hpp            \
	../flatsurf/local.hpp                                       \
	../flatsurf/moveable.hpp                                    \
	../flatsurf/odd_half_edge_map.hpp                           \
	../flatsurf/orientation.hpp                                 \
	../flatsurf/path.hpp                                        \
	../flatsurf/path_iterator.hpp                               \
	../flatsurf/permutation.hpp                                 \
	../flatsurf/saddle_connection.hpp                           \
	../flatsurf/saddle_connections.hpp                          \
	../flatsurf/saddle_connections_iterator.hpp                 \
	../flatsurf/serializable.hpp                                \
	../flatsurf/tracked.hpp                                     \
	../flatsurf/vector.hpp                                      \
	../flatsurf/vertex.hpp                                      \
	../flatsurf/vertical.hpp

nobase_pkginclude_HEADERS +=                                        \
	../flatsurf/detail/vector_base.hpp                          \
	../flatsurf/detail/vector_exact.hpp                         \
	../flatsurf/detail/vector_with_error.hpp                    \
	../flatsurf/external/spimpl/spimpl.h

noinst_HEADERS =                                                    \
	impl/approximation.hpp                                      \
	impl/chain.impl.hpp                                         \
	impl/chain_iterator.impl.hpp                                \
	impl/chain_vector.hpp                                       \
	impl/collapsed_half_edge.hpp                                \
	impl/contour_component.impl.hpp                             \
	impl/contour_component_state.hpp                            \
	impl/contour_connection.impl.hpp                            \
	impl/contour_decomposition.impl.hpp                         \
	impl/contour_decomposition_state.hpp                        \
	impl/edge_map.impl.hpp                                      \
	impl/edge_set.impl.hpp                                      \
	impl/edge_set_iterator.impl.hpp                             \
	impl/flat_triangulation_collapsed.impl.hpp                  \
	impl/flat_triangulation_combinatorial.impl.hpp              \
	impl/flat_triangulation.impl.hpp                            \
	impl/flow_component.impl.hpp                                \
	impl/flow_component_state.hpp                               \
	impl/flow_connection.impl.hpp                               \
	impl/flow_decomposition.impl.hpp                            \
	impl/flow_decomposition_state.hpp                           \
	impl/flow_triangulation.impl.hpp                            \
	impl/forward.hpp                                            \
	impl/half_edge_set.impl.hpp                                 \
	impl/half_edge_set_iterator.impl.hpp                        \
	impl/indexed_map.hpp                                        \
	impl/indexed_set.hpp                                        \
	impl/indexed_set_iterator.hpp                               \
	impl/interval_exchange_transformation.impl.hpp              \
	impl/lengths.hpp                                            \
	impl/path.impl.hpp                                          \
	impl/path_iterator.impl.hpp                                 \
	impl/quadratic_polynomial.hpp                               \
	impl/saddle_connection.impl.hpp                             \
	impl/saddle_connections.impl.hpp                            \
	impl/saddle_connections_iterator.impl.hpp                   \
	impl/tracked.impl.hpp                                       \
	impl/tracking_half_edge.impl.hpp                            \
	impl/tracking.impl.hpp                                      \
	impl/tracking_map.impl.hpp                                  \
	impl/tracking_storage.hpp                                   \
	impl/vertex.impl.hpp                                        \
	impl/vertical.impl.hpp                                      \
	util/assert.ipp                                             \
	util/false.ipp                                              \
	util/hash.ipp                                               \
	util/instance_of.ipp                                        \
	util/instantiate.ipp                                        \
	util/union_find.ipp                                         \
	vector/algorithm/exact.ipp                                  \
	vector/algorithm/exact.extension.ipp                        \
	vector/algorithm/extension.ipp                              \
	vector/algorithm/base.ipp                                   \
	vector/algorithm/base.extension.ipp                         \
	vector/algorithm/with_error.ipp                             \
	vector/algorithm/with_error.extension.ipp                   \
	vector/storage/forward.ipp                                  \
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
# we use libfmt for string formatting
libflatsurf_la_LDFLAGS += -lfmt

$(builddir)/../flatsurf/local.hpp: $(srcdir)/../flatsurf/local.hpp.in Makefile
	mkdir -p $(builddir)/../flatsurf
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate local.hpp, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Generate local.hpp before performing any compilation
BUILT_SOURCES = ../flatsurf/local.hpp
# Distribute local.hpp.in as part of the tarball, but not local.hpp
EXTRA_DIST = ../flatsurf/local.hpp.in
nodist_include_HEADERS = ../flatsurf/local.hpp

# We vendor the header-only library slimsig
EXTRA_DIST += external/slimsig/include
# We vendor the header-only library rx-ranges
EXTRA_DIST += external/rx-ranges/include/rx/ranges.hpp
# We vendor the header-only library gmpxxll
EXTRA_DIST += external/gmpxxll/gmpxxll/mpz_class.hpp

CLEANFILES = ../flatsurf/local.hpp
