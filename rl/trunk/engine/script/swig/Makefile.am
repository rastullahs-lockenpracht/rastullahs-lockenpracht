CLEANFILES = RlExports.cxx

all: RlExports.cxx

RlExports.cxx: RlCore.head.swig RlCore.swig RlRules.head.swig \
 RlRules.swig RlUi.head.swig RlUi.swig RlAi.head.swig RlAi.swig \
 RlScript.head.swig RlScript.swig RlCommon.head.swig RlCommon.swig \
 RlExports.i TypeMaps.i RlExports.i \
 TypeDynamicDirector.swig TypeOgreColourValue.swig TypeOgreQuaternion.swig \
 TypeOgreRadian.swig TypeOgreReal.swig TypeOgreString.swig TypeOgreStringVector.swig \
 TypeOgreVector3.swig TypeRlCeGuiString.swig TypeRlCeGuiStringVector.swig \
 TypeRlProperty.swig TypeStdPairIntInt.swig
 
.i.cxx:
	$(SWIG) $(SWIG_RUBY_OPT) -o $@ $<
