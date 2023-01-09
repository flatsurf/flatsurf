lib_LTLIBRARIES = libflatsurf.la

libflatsurf_la_SOURCES =                                            \
	approximation.cc                                            \
	assert_connection.cc                                        \
	bound.cc                                                    \
	ccw.cc                                                      \
	chain.cc                                                    \
	chain_iterator.cc                                           \
	chain_vector.cc                                             \
	collapsed_half_edge.cc                                      \
	combinatorial_equivalence.cc                                \
	combinatorial_equivalence_class_code.cc                     \
	combinatorial_equivalence_walker.cc                         \
	combinatorial_deformation_relation.cc                       \
	composite_deformation_relation.cc                           \
	contour_component.cc                                        \
	contour_component_state.cc                                  \
	contour_connection.cc                                       \
	contour_decomposition.cc                                    \
	contour_decomposition_state.cc                              \
	deformation.cc                                              \
	deformation_relation.cc                                     \
	edge.cc                                                     \
	edge_set.cc                                                 \
	edge_set_iterator.cc                                        \
	equivalence.cc                                              \
	equivalence_class.cc                                        \
	equivalence_class_code.cc                                   \
	flat_triangulation.cc                                       \
	flat_triangulation_collapsed.cc                             \
	flat_triangulation_combinatorial.cc                         \
	flat_triangulation_combinatorics.cc                         \
	flip_deformation_relation.cc                                \
	flow_component.cc                                           \
	flow_component_state.cc                                     \
	flow_connection.cc                                          \
	flow_decomposition.cc                                       \
	flow_decomposition_state.cc                                 \
	flow_triangulation.cc                                       \
	generic_retriangulation_deformation_relation.cc             \
	half_edge.cc                                                \
	half_edge_intersection.cc                                   \
	half_edge_set.cc                                            \
	half_edge_set_iterator.cc                                   \
	indexed_set.cc                                              \
	indexed_set_iterator.cc                                     \
	insert_marked_point_on_edge_deformation_relation.cc         \
	insert_marked_point_in_face_deformation_relation.cc         \
	interval_exchange_transformation.cc                         \
	lengths.cc                                                  \
	linear_deformation_relation.cc                              \
	linear_equivalence.cc                                       \
	linear_equivalence_class_code.cc                            \
	linear_equivalence_walker.cc                                \
	orientation.cc                                              \
	path.cc                                                     \
	path_iterator.cc                                            \
	permutation.cc                                              \
	point.cc                                                    \
	quadratic_polynomial.cc                                     \
	read_only.cc                                                \
	saddle_connection.cc                                        \
	saddle_connections.cc                                       \
	saddle_connections_by_length.cc                             \
	saddle_connections_by_length_iterator.cc                    \
	saddle_connections_iterator.cc                              \
	saddle_connections_sample.cc                                \
	saddle_connections_sample_iterator.cc                       \
	shift_deformation_relation.cc                               \
	slit_deformation_relation.cc                                \
	tracked.cc                                                  \
	trivial_deformation_relation.cc                             \
	vector.cc                                                   \
	vertex.cc                                                   \
	vertical.cc                                                 \
	weak_read_only.cc

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
	../flatsurf/deformation.hpp                                 \
	../flatsurf/delaunay.hpp                                    \
	../flatsurf/detail/vector_base.hpp                          \
	../flatsurf/detail/vector_exact.hpp                         \
	../flatsurf/detail/vector_with_error.hpp                    \
	../flatsurf/edge.hpp                                        \
	../flatsurf/edge_map.hpp                                    \
	../flatsurf/edge_set.hpp                                    \
	../flatsurf/edge_set_iterator.hpp                           \
	../flatsurf/equivalence.hpp                                 \
	../flatsurf/equivalence_class.hpp                           \
	../flatsurf/flat_triangulation.hpp                          \
	../flatsurf/flat_triangulation_collapsed.hpp                \
	../flatsurf/flat_triangulation_combinatorial.hpp            \
	../flatsurf/flat_triangulation_combinatorics.hpp            \
	../flatsurf/flatsurf.hpp                                    \
	../flatsurf/flow_component.hpp                              \
	../flatsurf/flow_connection.hpp                             \
	../flatsurf/flow_decomposition.hpp                          \
	../flatsurf/flow_triangulation.hpp                          \
	../flatsurf/fmt.hpp                                         \
	../flatsurf/forward.hpp                                     \
	../flatsurf/half_edge.hpp                                   \
	../flatsurf/half_edge_intersection.hpp                      \
	../flatsurf/half_edge_map.hpp                               \
	../flatsurf/half_edge_set.hpp                               \
	../flatsurf/half_edge_set_iterator.hpp                      \
	../flatsurf/interval_exchange_transformation.hpp            \
	../flatsurf/isomorphism.hpp                                 \
	../flatsurf/local.hpp                                       \
	../flatsurf/managed_movable.hpp                             \
	../flatsurf/movable.hpp                                     \
	../flatsurf/odd_half_edge_map.hpp                           \
	../flatsurf/orientation.hpp                                 \
	../flatsurf/path.hpp                                        \
	../flatsurf/path_iterator.hpp                               \
	../flatsurf/permutation.hpp                                 \
	../flatsurf/point.hpp                                       \
	../flatsurf/saddle_connection.hpp                           \
	../flatsurf/saddle_connections.hpp                          \
	../flatsurf/saddle_connections_by_length.hpp                \
	../flatsurf/saddle_connections_iterator.hpp                 \
	../flatsurf/saddle_connections_by_length_iterator.hpp       \
	../flatsurf/saddle_connections_sample.hpp                   \
	../flatsurf/saddle_connections_sample_iterator.hpp          \
	../flatsurf/serializable.hpp                                \
	../flatsurf/tracked.hpp                                     \
	../flatsurf/vector.hpp                                      \
	../flatsurf/vertex.hpp                                      \
	../flatsurf/vertical.hpp

nobase_pkginclude_HEADERS +=                                        \
	../flatsurf/external/spimpl/spimpl.h

noinst_HEADERS =                                                    \
	impl/approximation.hpp                                      \
	impl/assert_connection.hpp                                  \
	impl/chain.impl.hpp                                         \
	impl/chain_iterator.impl.hpp                                \
	impl/chain_vector.hpp                                       \
	impl/collapsed_half_edge.hpp                                \
	impl/combinatorial_deformation_relation.hpp                 \
	impl/combinatorial_equivalence.hpp                          \
	impl/combinatorial_equivalence_class_code.hpp               \
	impl/combinatorial_equivalence_walker.hpp                   \
	impl/composite_deformation_relation.hpp                     \
	impl/contour_component.impl.hpp                             \
	impl/contour_component_state.hpp                            \
	impl/contour_connection.impl.hpp                            \
	impl/contour_decomposition.impl.hpp                         \
	impl/contour_decomposition_state.hpp                        \
	impl/deformation.impl.hpp                                   \
	impl/deformation_relation.hpp                               \
	impl/edge_map.impl.hpp                                      \
	impl/edge_set.impl.hpp                                      \
	impl/edge_set_iterator.impl.hpp                             \
	impl/equivalence.impl.hpp                                   \
	impl/equivalence_class.impl.hpp                             \
	impl/equivalence_class_code.hpp                             \
	impl/equivalence_walker.hpp                                 \
	impl/flat_triangulation_collapsed.impl.hpp                  \
	impl/flat_triangulation_combinatorial.impl.hpp              \
	impl/flat_triangulation_combinatorics.impl.hpp              \
	impl/flat_triangulation.impl.hpp                            \
	impl/flip_deformation_relation.hpp                          \
	impl/flow_component.impl.hpp                                \
	impl/flow_component_state.hpp                               \
	impl/flow_connection.impl.hpp                               \
	impl/flow_decomposition.impl.hpp                            \
	impl/flow_decomposition_state.hpp                           \
	impl/flow_triangulation.impl.hpp                            \
	impl/forward.hpp                                            \
	impl/generic_retriangulation_deformation_relation.hpp       \
	impl/half_edge_intersection.impl.hpp                        \
	impl/half_edge_set.impl.hpp                                 \
	impl/half_edge_set_iterator.impl.hpp                        \
	impl/indexed_map.hpp                                        \
	impl/indexed_set.hpp                                        \
	impl/indexed_set_iterator.hpp                               \
	impl/insert_marked_point_on_edge_deformation_relation.hpp   \
	impl/insert_marked_point_in_face_deformation_relation.hpp   \
	impl/interval_exchange_transformation.impl.hpp              \
	impl/lengths.hpp                                            \
	impl/linear_equivalence.hpp                                 \
	impl/linear_equivalence_class.hpp                           \
	impl/linear_equivalence_walker.hpp                          \
	impl/linear_deformation_relation.hpp                        \
	impl/managed_movable.impl.hpp                               \
	impl/path.impl.hpp                                          \
	impl/path_iterator.impl.hpp                                 \
	impl/point.impl.hpp                                         \
	impl/quadratic_polynomial.hpp                               \
	impl/read_only.hpp                                          \
	impl/saddle_connection.impl.hpp                             \
	impl/saddle_connections.impl.hpp                            \
	impl/saddle_connections_by_length.impl.hpp                  \
	impl/saddle_connections_iterator.impl.hpp                   \
	impl/saddle_connections_by_length_iterator.impl.hpp         \
	impl/saddle_connections_sample.impl.hpp                     \
	impl/saddle_connections_sample_iterator.impl.hpp            \
	impl/shift_deformation_relation.hpp                         \
	impl/slit_deformation_relation.hpp                          \
	impl/tracked.impl.hpp                                       \
	impl/trivial_deformation_relation.hpp                       \
	impl/vector.impl.hpp                                        \
	impl/vertex.impl.hpp                                        \
	impl/vertical.impl.hpp                                      \
	impl/weak_read_only.hpp                                     \
	util/assert.ipp                                             \
	util/false.ipp                                              \
	util/hash.ipp                                               \
	util/instance_of.ipp                                        \
	util/instantiate.ipp                                        \
	util/streamed.ipp                                           \
	util/union_find.ipp

libflatsurf_la_LDFLAGS = -version-info $(libflatsurf_version_info)
# some of our vectors use arb directly and through exact-real's arb wrappers
libflatsurf_la_LDFLAGS += -larb
# we use exact-real in our vectors
libflatsurf_la_LDFLAGS += -lexactreal
# we use e-antic in our vectors
libflatsurf_la_LDFLAGS += -leanticxx -leantic
# we build IETs with intervalxt
libflatsurf_la_LDFLAGS += -lintervalxt

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

# We vendor the header-only library sigslot
EXTRA_DIST += external/sigslot/include
# We vendor the header-only library rx-ranges
EXTRA_DIST += external/rx-ranges/include/rx/ranges.hpp
# We vendor our header-only library gmpxxll
EXTRA_DIST += external/gmpxxll/gmpxxll/mpz_class.hpp

CLEANFILES = ../flatsurf/local.hpp
