SUBDIRS =  engine tools docs

doc:
	cd docs; ${MAKE} doc
	
install-data-local:
	install -d /etc/rl
	install -m 644 rl.conf /etc/rl
		
