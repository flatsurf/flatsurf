lib_LTLIBRARIES = libflatsurf.la

libflatsurf_la_SOURCES =                 \
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

libflatsurf_la_LDFLAGS = -version-info $(libflatsurf_version_info)

$(builddir)/libflatsurf/libflatsurf.hpp: $(srcdir)/libflatsurf/libflatsurf.hpp.in Makefile
	mkdir -p $(builddir)/libflatsurf
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate libflatsurf.hpp, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Generate libflatsurf.hpp before performing any compilation
BUILT_SOURCES = libflatsurf/libflatsurf.hpp
# Distribute libflatsurf.hpp.in as part of the tarball, but not libflatsurf.hpp
EXTRA_DIST = libflatsurf/libflatsurf.hpp.in
nodist_include_HEADERS = libflatsurf/libflatsurf.hpp

CLEANFILES = libflatsurf/libflatsurf.hpp
