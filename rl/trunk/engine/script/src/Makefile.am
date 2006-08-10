INCLUDES = $(CEGUI_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) \
	$(STLPORT_CFLAGS) -I../include -I../../common/include -I../../core/include \
	-I../../dialog/include -I../../rules/include -I../../ui/include \
	-I../../ai/include

lib_LTLIBRARIES = libRlScript.la
libRlScript_la_LDFLAGS = $(CEGUI_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS)  \
	-Wl,--defsym -Wl,Init_libRlScript=Init_RlScript \
	-lRlCore -lRlCommon -lRlDialog -lRlUi -lRlRules \
	-L../../core/src  -L../../common/src -L../../dialog/src -L../../ui/src -L../../rules/src 
	
libRlScript_la_SOURCES = \
  ScriptSubsystem.cpp \
  ScriptObjectMarker.cpp \
	../swig/RlExports.cxx 
