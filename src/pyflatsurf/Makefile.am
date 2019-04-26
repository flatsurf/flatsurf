all-local:
	(cd $(srcdir); $(PYTHON) setup.py build --build-base $(builddir) --verbose)

install-exec-local:
	$(PYTHON) setup.py build --build-base $(builddir) install --prefix $(prefix) --single-version-externally-managed --record $(pythondir)/pyflatsurf/install_files.txt --verbose

uninstall-local:
	cat $(pythondir)/pyflatsurf/install_files.txt | xargs rm -rf
	rm -rf $(pythondir)/pyflatsurf
