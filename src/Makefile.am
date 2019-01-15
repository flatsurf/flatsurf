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

include_HEADERS =    \
    algebraic.h \
    alg_t.h \
    config.h \
    defs.h \
    psimplex.h \
    saddleconf.h \
    slice.h \
    slice-pvm.h \
    two_complex.h
