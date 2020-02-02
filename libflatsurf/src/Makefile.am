lib_LTLIBRARIES = libflatsurf.la

libflatsurf_la_SOURCES =                                            \
	ccw.cc                                                      \
	bound.cc                                                    \
	delaunay_triangulation.cc                                   \
	edge.cc                                                     \
	flat_triangulation.cc                                       \
	flat_triangulation_combinatorial.cc                         \
	half_edge.cc                                                \
	tracking_map.cc                                             \
	tracking_set.cc                                             \
	tracking.cc                                                 \
	tracking_half_edge.cc                                       \
	tracking_storage.cc                                         \
	contour_decomposition.cc                                    \
	permutation.cc                                              \
	saddle_connection.cc                                        \
	saddle_connections.cc                                       \
	flow_connection.cc                                          \
	vertex.cc                                                   \
	vector/vector.cc                                            \
	flow_decomposition.cc                                       \
	vertical.cc                                                 \
	flat_triangulation_collapsed.cc                             \
	flow_component.cc                                           \
	contour_component.cc                                        \
	interval_exchange_transformation.cc                         \
	lengths.cc                                                  \
	flow_decomposition_state.cc                                 \
	flow_component_state.cc                                     \
	contour_decomposition_state.cc                              \
	collapsed_half_edge.cc                                      \
	contour_connection.cc                                       \
	chain.cc

nobase_include_HEADERS =                                            \
	../flatsurf/bound.hpp                                       \
	../flatsurf/ccw.hpp                                         \
	../flatsurf/flow_component.hpp                              \
	../flatsurf/decomposition_step.hpp                          \
	../flatsurf/cppyy.hpp                                       \
	../flatsurf/delaunay_triangulation.hpp                      \
	../flatsurf/edge.hpp                                        \
	../flatsurf/flatsurf.hpp                                    \
	../flatsurf/local.hpp                                       \
	../flatsurf/flat_triangulation.hpp                          \
	../flatsurf/flat_triangulation_combinatorial.hpp            \
	../flatsurf/flat_triangulation_collapsed.hpp                \
	../flatsurf/flow_decomposition.hpp                          \
	../flatsurf/forward.hpp                                     \
	../flatsurf/chain.hpp                                       \
	../flatsurf/half_edge.hpp                                   \
	../flatsurf/half_edge_map.hpp                               \
	../flatsurf/contour_decomposition.hpp                       \
	../flatsurf/contour_component.hpp                           \
	../flatsurf/contour_connection.hpp                          \
	../flatsurf/orientation.hpp                                 \
	../flatsurf/permutation.hpp                                 \
	../flatsurf/saddle_connections.hpp                          \
	../flatsurf/saddle_connection.hpp                           \
	../flatsurf/vector.hpp                                      \
	../flatsurf/cereal.hpp                                      \
	../flatsurf/vertex.hpp                                      \
	../flatsurf/vertical.hpp                                    \
	../flatsurf/vertex_map.hpp

nobase_include_HEADERS +=                                           \
	../flatsurf/detail/vector_base.hpp                          \
	../flatsurf/detail/vector_exact.hpp                         \
	../flatsurf/detail/vector_with_error.hpp                    \
	../flatsurf/external/spimpl/spimpl.h

noinst_HEADERS =                                                    \
	impl/flow_component.impl.hpp                                \
	impl/contour_component.impl.hpp                             \
	impl/contour_decomposition.impl.hpp                         \
	impl/flat_triangulation_collapsed.impl.hpp                  \
	impl/flat_triangulation_combinatorial.impl.hpp              \
	util/assert.ipp                                             \
	util/false.ipp                                              \
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

CLEANFILES = ../flatsurf/local.hpp