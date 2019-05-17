bin_PROGRAMS = flatsurf

flatsurf_SOURCES =        \
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

noinst_HEADERS =          \
    algebraic.h           \
    alg_t.h               \
    big_point.h           \
    cylinder.h            \
    defs.h                \
    dir.h                 \
    dmap.h                \
    elementary_geometry.h \
    face.h                \
    globals.h             \
    my_ostream.h          \
    number_field.h        \
    oedge.h               \
    params.h              \
    pface.h               \
    poly.h                \
    psimplex.h            \
    puedge.h              \
    pvertex.h             \
    saddleconf.h          \
    segment.h             \
    shared.h              \
    simplex.h             \
    summary.h             \
    two_complex.h         \
    uedge.h               \
    vector.h              \
    vertex.h              \
    vert_pattern.h


AM_LDFLAGS = $(builddir)/../libflatsurf/libflatsurf.la
# is_fmpq() from eanticxx is calling directly into FLINT
AM_LDFLAGS += -lflint
# we are manipulating mpz_t and mpq_t directly and through GMPXX
AM_LDFLAGS += -lgmp -lgmpxx
# we are using NTL's LLL
AM_LDFLAGS += -lntl
AM_CPPFLAGS = -I $(srcdir)/../../src/libflatsurf/ -I $(builddir)/../../src/libflatsurf -isystem $(srcdir)/external
