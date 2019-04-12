all-local:
	(cd $(srcdir); $(PYTHON) setup.py build --build-base $(builddir)/build --verbose)

install-exec-local:
	$(PYTHON) $(srcdir)/setup.py install --prefix $(DESTDIR)$(prefix) --single-version-externally-managed --record $(DESTDIR)$(pkgpythondir)/install_files.txt --verbose

uninstall-local:
	cat $(DESTDIR)$(pkgpythondir)/install_files.txt | xargs rm -rf
	rm -rf $(DESTDIR)$(pkgpythondir)
