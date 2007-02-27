CLEANFILES = RlExports.cxx

all: RlExports.cxx

RlExports.cxx: RlCore.head.swig RlCore.swig RlRules.head.swig \
 RlRules.swig RlUi.head.swig RlUi.swig RlAi.head.swig RlAi.swig \
 RlScript.head.swig RlScript.swig RlCommon.head.swig RlCommon.swig \
 RlDialog.head.swig RlDialog.swig RlExports.i TypeMaps.i
 
.i.cxx:
	$(SWIG) $(SWIG_RUBY_OPT) -o $@ $<
