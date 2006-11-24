SUBDIRS =   engine plugins tools docs
if with_tests 
  SUBDIRS += tests
endif

doc:
	cd docs; ${MAKE} doc
	
install-data-local:
	install -d /etc/rastullah
	install -m 644 rastullah.conf /etc/rastullah
		
