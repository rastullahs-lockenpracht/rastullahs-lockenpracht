SUBDIRS =   engine plugins tools docs
if with_tests 
  SUBDIRS += tests
endif

doc:
	cd docs; ${MAKE} doc
	
install-data-local:
	@if [ "$(enable_global_config)" == "yes" ]; then \
		install -d /etc/rastullah; \
		install -m 644 rastullah.conf /etc/rastullah; \
	else \
		echo Nothing to install here; \
	fi 

uninstall-local:
	@if [ "$(enable_global_config)" == "yes" ]; then \
		rm -rf /etc/rastullah; \
	else \
		echo Nothing to uninstall here; \
	fi 
