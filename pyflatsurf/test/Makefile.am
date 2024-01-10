if HAVE_SAGE
  MAYBE_SAGE = sage-doctest.sh flow_decomposition.py
endif
TESTS = bound.py deformation.py equivalence_class.py flat_triangulation.py flat_triangulation_combinatorial.py half_edge.py point.py saddle_connections.py vector_renf_elem.py vector_gmp.py vertex.py python-doctest.sh $(MAYBE_SAGE)
EXTRA_DIST = $(TESTS) surfaces.py

AM_TESTS_ENVIRONMENT = . $(builddir)/test-env.sh;

bound.py: test-env.sh bin/python
half_edge.py: test-env.sh bin/python
equivalence_class.py: test-env.sh surfaces.py bin/python
flat_triangulation.py: test-env.sh surfaces.py bin/python
flat_triangulation_combinatorial.py: test-env.sh surfaces.py bin/python
point.py: test-env.sh surfaces.py bin/python
saddle_connections.py: test-env.sh surfaces.py bin/python
vector_renf_elem.py: test-env.sh surfaces.py bin/python
vector_gmp.py: test-env.sh bin/python
deformation.py: test-env.sh surfaces.py bin/python
vertex.py: test-env.sh surfaces.py bin/python
python-doctest.sh: test-env.sh bin/python
sage-doctest.sh: test-env.sh bin/python

@VALGRIND_CHECK_RULES@

BUILT_SOURCES = test-env.sh bin/python python-doctest.sh sage-doctest.sh
EXTRA_DIST += test-env.sh.in bin/python.in python-doctest.sh.in sage-doctest.sh.in disable-pytest/pytest.py
CLEANFILES = test-env.sh bin/python python-doctest.sh sage-doctest.sh

$(builddir)/bin/python: $(srcdir)/bin/python.in Makefile
	mkdir -p $(builddir)/bin
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' -e 's,[@]pythondir[@],$(pythondir),g' < $< > $@
	chmod +x $@

$(builddir)/test-env.sh: $(srcdir)/test-env.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' -e 's,[@]pythondir[@],$(pythondir),g' < $< > $@

$(builddir)/python-doctest.sh: $(srcdir)/python-doctest.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' < $< > $@
	chmod +x $@

$(builddir)/sage-doctest.sh: $(srcdir)/sage-doctest.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' < $< > $@
	chmod +x $@
