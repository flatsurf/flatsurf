bin_PROGRAMS = flatsurf

flatsurf_SOURCES =         \
    main.cc               \
    algebraic.cc          \
    alg_misc.cc           \
    alg_t.cc              \
    dir.cc                \
    face.cc               \
    input.cc              \
    math.cc               \
    misc.cc               \
    oedge.cc              \
    perturb.cc            \
    psimplex.cc           \
    reflect.cc            \
    report.cc             \
    sweep.cc              \
    triang.cc             \
    two_complex.cc        \
    uedge.cc              \
    vertex.cc             \
    params.cc             \
    globals.cc

AM_LDFLAGS = $(builddir)/../libflatsurf/libflatsurf.la
AM_CPPFLAGS = -I $(srcdir)/../../src/libflatsurf/ -I $(builddir)/../../src/libflatsurf -isystem $(srcdir)/external
