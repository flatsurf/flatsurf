if HAVE_SAGE
  MAYBE_SAGE = sage-doctest.sh
endif
TESTS = saddle_connections.py vector_renf_elem.py vector_gmp.py python-doctest.sh $(MAYBE_SAGE)
EXTRA_DIST = $(TESTS) surfaces.py

AM_TESTS_ENVIRONMENT = . $(builddir)/test-env.sh;

saddle_connections.py: test-env.sh surfaces.py
vector_renf_elem.py: test-env.sh surfaces.py
vector_gmp.py: test-env.sh
python-doctest.sh: test-env.sh
sage-doctest.sh: test-env.sh

@VALGRIND_CHECK_RULES@

BUILT_SOURCES = test-env.sh python-doctest.sh sage-doctest.sh
EXTRA_DIST += test-env.sh.in python-doctest.sh.in sage-doctest.sh.in
CLEANFILES = test-env.sh python-doctest.sh sage-doctest.sh
$(builddir)/test-env.sh: $(srcdir)/test-env.sh.in Makefile
	sed -e 's,[@]srcdir[@],$(srcdir),g' -e 's,[@]builddir[@],$(builddir),g' -e 's,[@]pythondir[@],$(pythondir),g' < $< > $@
$(builddir)/python-doctest.sh: $(srcdir)/python-doctest.sh.in Makefile
	sed -e 's,[@]srcdir[@],$(srcdir),g' -e 's,[@]builddir[@],$(builddir),g' < $< > $@
	chmod +x $@
$(builddir)/sage-doctest.sh: $(srcdir)/sage-doctest.sh.in Makefile
	sed -e 's,[@]srcdir[@],$(srcdir),g' -e 's,[@]builddir[@],$(builddir),g' < $< > $@
	chmod +x $@
