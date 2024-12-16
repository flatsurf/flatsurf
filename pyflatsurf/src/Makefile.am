all-local:
	mkdir -p $(abs_builddir)/build
	cd $(abs_srcdir) && $(PYTHON) $(abs_srcdir)/setup.py build --verbose --build-base $(abs_builddir)/build

install-exec-local:
	$(PYTHON) $(abs_srcdir)/setup.py install --prefix $(DESTDIR)$(prefix) --single-version-externally-managed --record $(DESTDIR)$(pythondir)/pyflatsurf/install_files.txt --verbose

uninstall-local:
	cat $(DESTDIR)$(pythondir)/pyflatsurf/install_files.txt | xargs rm -rf
	rm -rf $(DESTDIR)$(pythondir)/pyflatsurf

clean-local:
	-rm -rf pyflatsurf/__pycache__ pyflatsurf.egg-info build .pytest_cache

EXTRA_DIST = setup.py pyflatsurf/__init__.py pyflatsurf/cppyy_flatsurf.py pyflatsurf/factory.py pyflatsurf/__init__.py pyflatsurf/pythonization.py pyflatsurf/vector.py
