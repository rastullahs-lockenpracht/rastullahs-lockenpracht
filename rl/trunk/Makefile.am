SUBDIRS =  engine tools docs

doc:
	cd docs; ${MAKE} doc
	
tests:
	cd engine; ${MAKE} tests

install-data-local:
	install -d /etc/rl
	install -m 644 rl.conf /etc/rl
		
