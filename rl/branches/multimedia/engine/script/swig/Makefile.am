CLEANFILES = RlExports.cxx

all: RlExports.cxx

RlExports.cxx: RlCore.head.inc RlCore.inc RlRules.head.inc \
 RlRules.inc RlMultimedia.head.inc RlMultimedia.inc RlUi.head.inc RlUi.inc \
 RlScript.head.inc RlScript.inc
 
.i.cxx:
	$(SWIG) $(SWIG_RUBY_OPT) -o $@ $<