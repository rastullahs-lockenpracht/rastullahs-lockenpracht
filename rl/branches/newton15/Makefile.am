SUBDIRS =  engine tools docs
if with_tests 
  SUBDIRS += tests
endif

doc:
	cd docs; ${MAKE} doc
	
install-data-local:
	install -d /etc/rl
	install -m 644 rl.conf /etc/rl
		
