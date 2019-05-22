lib_LTLIBRARIES = libflatsurf.la

libflatsurf_la_SOURCES =                                      \
	half_edge.cc                                                \
	vertex.cc                                                   \
	edge.cc                                                     \
	flat_triangulation.cc                                       \
	delaunay_triangulation.cc                                   \
	flat_triangulation_combinatorial.cc                         \
	saddle_connections.cc                                       \
	permutation.cc                                              \
	bound.cc                                                    \
	ccw.cc                                                      \
	half_edge_map.cc                                            \
	cppyy.cc                                                    \
	interval_exchange_transformation.cc                         \
	vector/vector_longlong.cc                                   \
	vector/vector_along_triangulation.cc                        \
	vector/vector_exactreal.cc                                  \
	vector/vector_eantic.cc                                     \
	vector/vector_arb.cc                                        \
	vector/vector_along_triangulation_with_approximation.cc

nobase_include_HEADERS =                                     \
	flatsurf/assert.hpp                                        \
	flatsurf/bound.hpp                                         \
	flatsurf/ccw.hpp                                           \
	flatsurf/cppyy.hpp                                         \
	flatsurf/edge.hpp                                          \
	flatsurf/flatsurf.hpp                                      \
	flatsurf/flat_triangulation_combinatorial.hpp              \
	flatsurf/flat_triangulation.hpp                            \
	flatsurf/delaunay_triangulation.hpp                        \
	flatsurf/forward.hpp                                       \
	flatsurf/half_edge.hpp                                     \
	flatsurf/half_edge_map.hpp                                 \
	flatsurf/orientation.hpp                                   \
	flatsurf/permutation.hpp                                   \
	flatsurf/saddle_connection.hpp                             \
	flatsurf/saddle_connections.hpp                            \
	flatsurf/saddle_connections_iterator.hpp                   \
	flatsurf/vector_along_triangulation.hpp                    \
	flatsurf/vector_along_triangulation_with_approximation.hpp \
	flatsurf/vector_arb.hpp                                    \
	flatsurf/vector_eantic.hpp                                 \
	flatsurf/vector_exactreal.hpp                              \
	flatsurf/vector_longlong.hpp                               \
	flatsurf/vertex.hpp                                        \
	flatsurf/interval_exchange_transformation.hpp              \
	external/spimpl/spimpl.h

noinst_HEADERS =                                             \
	flatsurf/detail/as_vector.hpp                              \
	saddle_connections_iterator.ipp                            \
	saddle_connection.ipp

libflatsurf_la_LDFLAGS = -version-info $(libflatsurf_version_info)
# some of our vectors use arb directly and through exact-real's arb wrappers
libflatsurf_la_LDFLAGS += -larb
# we use exact-real in our vectors
libflatsurf_la_LDFLAGS += -lexactreal
# we use e-antic in our vectors
libflatsurf_la_LDFLAGS += -leanticxx -leantic

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
