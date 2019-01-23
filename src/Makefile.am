bin_PROGRAMS = polygon

polygon_SOURCES = main.cc

polygon_LDADD = libpolygon.la

lib_LTLIBRARIES = libpolygon.la

libpolygon_la_SOURCES = \
    algebraic.cc     \
    alg_misc.cc      \
    alg_t.cc         \
    dir.cc           \
    face.cc          \
    input.cc         \
    math.cc          \
    misc.cc          \
    oedge.cc         \
    perturb.cc       \
    psimplex.cc      \
    reflect.cc       \
    report.cc        \
    slice.cc         \
    slice-pvm.cc     \
    sweep.cc         \
    triang.cc        \
    two_complex.cc   \
    uedge.cc         \
    vertex.cc        \
    globals.cc

libpolygon_la_LDFLAGS = -version-info $(libpolygon_version_info)

libpolygonincludedir = $(includedir)/libpolygon
libpolygoninclude_HEADERS =  \
    libpolygon/algebraic.h   \
    libpolygon/alg_t.h       \
    libpolygon/defs.h        \
    libpolygon/psimplex.h    \
    libpolygon/saddleconf.h  \
    libpolygon/slice.h       \
    libpolygon/slice-pvm.h   \
    libpolygon/two_complex.h \
    libpolygon/globals.h     \
    libpolygon/libpolygon.h

$(builddir)/libpolygon/libpolygon.h: $(srcdir)/libpolygon/libpolygon.h.in Makefile
	mkdir -p $(builddir)/libpolygon
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate libpolygon.h, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Generate libpolygon.h before performing any compilation
BUILT_SOURCES = libpolygon/libpolygon.h
# Distribute libpolygon.h.in as part of the tarball, but not libpolygon.h
EXTRA_DIST = libpolygon/libpolygon.h.in
nodist_libpolygon_la_SOURCES = libpolygon/libpolygon.h

CLEANFILES = libpolygon/libpolygon.h
