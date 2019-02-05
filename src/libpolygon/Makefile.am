lib_LTLIBRARIES = libpolygon.la

libpolygon_la_SOURCES =                 \
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

libpolygon_la_LDFLAGS = -version-info $(libpolygon_version_info)

$(builddir)/libpolygon/libpolygon.hpp: $(srcdir)/libpolygon/libpolygon.hpp.in Makefile
	mkdir -p $(builddir)/libpolygon
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate libpolygon.hpp, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Generate libpolygon.hpp before performing any compilation
BUILT_SOURCES = libpolygon/libpolygon.hpp
# Distribute libpolygon.hpp.in as part of the tarball, but not libpolygon.hpp
EXTRA_DIST = libpolygon/libpolygon.hpp.in
nodist_include_HEADERS = libpolygon/libpolygon.hpp

CLEANFILES = libpolygon/libpolygon.hpp
