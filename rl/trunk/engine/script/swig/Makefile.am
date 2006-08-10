CLEANFILES = RlExports.cxx

all: RlExports.cxx

RlExports.cxx: RlCore.head.inc RlCore.inc RlRules.head.inc \
 RlRules.inc RlUi.head.inc RlUi.inc \
 RlScript.head.inc RlScript.inc RlCommon.head.inc RlCommon.inc \
 RlDialog.head.inc RlDialog.inc RlExports.i TypeMaps.i
 
.i.cxx:
	$(SWIG) $(SWIG_RUBY_OPT) -o $@ $<