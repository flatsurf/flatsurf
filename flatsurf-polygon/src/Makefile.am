bin_PROGRAMS = flatsurf-polygon

flatsurf_polygon_SOURCES =\
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

if HAVE_LOCAL_LIBFLATSURF
  MAYBE_LIBFLATSURF = $(builddir)/../../libflatsurf/src/libflatsurf.la
endif
AM_LDFLAGS = $(MAYBE_LIBFLATSURF)

# we use e-antic/exact-real in several places
AM_LDFLAGS += -lexactreal -leanticxx -leantic
# we use intervalxt in our main
AM_LDFLAGS += -lintervalxt
# is_fmpq() from eanticxx is calling directly into FLINT
AM_LDFLAGS += -lflint
# e-antic's renf_elem.h uses arb_mul_si
AM_LDFLAGS += -larb
# we are manipulating mpz_t and mpq_t directly and through GMPXX
AM_LDFLAGS += -lgmp -lgmpxx
# we are using NTL's LLL
AM_LDFLAGS += -lntl
AM_CPPFLAGS = -I $(srcdir)/../../libflatsurf/src -I $(builddir)/../../libflatsurf/src
