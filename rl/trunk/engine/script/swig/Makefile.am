CLEANFILES = RlExports.cxx

SWIG_FILES = \
	RlCore.head.inc \
	RlCore.inc \
	RlExports.i \
	RlRules.head.inc \
	RlRules.inc \
	RlScript.head.inc \
	RlScript.inc \
	RlSound.head.inc \
	RlSound.inc \
	RlUi.head.inc \
	RlUi.inc \
	TypeMaps.i
	
.i.cxx: $(SWIG_FILES)
	$(SWIG) $(SWIG_RUBY_OPT) -o RlExports.cxx RlExports.i
