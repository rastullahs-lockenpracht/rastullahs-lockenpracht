SUBDIRS = src

doc:
	cd src; ${MAKE} doc
	
deactivated-install-data-local: doc
	install -d ${pkgdatadir}/docs/html
	install -m 644 api/html/*.html ${pkgdatadir}/docs/html
	install -m 644 api/html/*.png ${pkgdatadir}/docs/html
	install -m 644 api/html/*.css ${pkgdatadir}/docs/html
	