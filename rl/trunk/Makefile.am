SUBDIRS =  engine modules tools docs

install:
	cd docs; ${MAKE} install
	cd engine; ${MAKE} install
	cd modules; ${MAKE} install
	cd tools; ${MAKE} install

doc:
	cd docs; ${MAKE} doc
	
tests:
	cd engine; ${MAKE} tests
