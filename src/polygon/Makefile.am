bin_PROGRAMS = polygon

polygon_SOURCES =         \
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

# TODO: Something is probably wrong with libexactreal - I should  not have to link FLINT
AM_LDFLAGS = $(builddir)/../libpolygon/libpolygon.la -lflint
AM_CPPFLAGS = -I $(srcdir)/../../src/libpolygon/ -I $(builddir)/../../src/libpolygon
