CLEANFILES = RlExports.cxx

all: RlExports.cxx RlCore.head.inc RlCore.inc RlRules.head.inc \
	RlRules.inc RlSound.head.inc RlSound.inc RlUi.head.inc RlUi.inc

.inc.i:

.i.cxx: 
	$(SWIG) $(SWIG_RUBY_OPT) -o $@ $<
