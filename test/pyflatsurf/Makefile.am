TESTS = saddle_connections.py vector_renf_elem.py vector_gmp.py
EXTRA_DIST = $(TESTS) surfaces.py

AM_TESTS_ENVIRONMENT = . $(builddir)/test-env.sh;

saddle_connections.py: test-env.sh surfaces.py
vector_renf_elem.py: test-env.sh surfaces.py
vector_gmp.py: test-env.sh

@VALGRIND_CHECK_RULES@

# Run tests serially so we can see the output without having to check the log file explicitly.
# AUTOMAKE_OPTIONS = serial-tests

BUILT_SOURCES = test-env.sh
EXTRA_DIST += test-env.sh.in
CLEANFILES = test-env.sh
$(builddir)/test-env.sh: $(srcdir)/test-env.sh.in Makefile
	sed -e 's,[@]srcdir[@],$(srcdir),g' -e 's,[@]builddir[@],$(builddir),g' -e 's,[@]pythondir[@],$(pythondir),g' < $< > $@
