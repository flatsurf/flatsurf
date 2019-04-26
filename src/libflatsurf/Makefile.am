lib_LTLIBRARIES = libflatsurf.la

libflatsurf_la_SOURCES =                \
    vector_longlong.cc                  \
    vector_along_triangulation.cc       \
    half_edge.cc                        \
    vertex.cc                           \
    edge.cc                             \
    flat_triangulation.cc               \
    flat_triangulation_combinatorial.cc \
    saddle_connections.cc               \
    saddle_connection.cc                \
    saddle_connections_iterator.cc      \
    permutation.cc                      \
    bound.cc                            \
    vector_eantic.cc                    \
    vector_arb.cc                       \
    ccw.cc                              \
    half_edge_map.cc                    \
    cppyy.cc                            \
    vector_exactreal.cc                 \
    vector_along_triangulation_with_approximation.cc

nobase_include_HEADERS =                              \
    flatsurf/assert.hpp                                        \
    flatsurf/bound.hpp                                         \
    flatsurf/ccw.hpp                                           \
    flatsurf/cppyy.hpp                                         \
    flatsurf/edge.hpp                                          \
    flatsurf/flatsurf.hpp                                      \
    flatsurf/flat_triangulation_combinatorial.hpp              \
    flatsurf/flat_triangulation.hpp                            \
    flatsurf/forward.hpp                                       \
    flatsurf/half_edge.hpp                                     \
    flatsurf/half_edge_map.hpp                                 \
    flatsurf/libflatsurf.hpp                                   \
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
    external/spimpl/spimpl.h

libflatsurf_la_LDFLAGS = -version-info $(libflatsurf_version_info)

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
