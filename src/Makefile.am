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
    vertex.cc 

libpolygon_la_LDFLAGS = -version-info $(libpolygon_version_info)

libpolygonincludedir = $(includedir)/libpolygon
libpolygoninclude_HEADERS =    \
    libpolygon/algebraic.h \
    libpolygon/alg_t.h \
    libpolygon/config.h \
    libpolygon/defs.h \
    libpolygon/psimplex.h \
    libpolygon/saddleconf.h \
    libpolygon/slice.h \
    libpolygon/slice-pvm.h \
    libpolygon/two_complex.h
