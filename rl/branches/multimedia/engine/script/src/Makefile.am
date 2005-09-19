INCLUDES = $(CEGUI_CFLAGS) $(Plugin_TheoraVideoSystem_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../core/include -I../../multimedia/include -I../../dialog/include -I../../rules/include -I../../ui/include

lib_LTLIBRARIES = libRlScript.la
libRlScript_la_LDFLAGS = $(CEGUI_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS)  \
	-Wl,--defsym -Wl,Init_libRlScript=Init_RlScript \
	-lRlCore -lRlCommon -lRlDialog -lRlUi -lRlRules -lRlMultimedia \
	-L../../core/src  -L../../common/src -L../../dialog/src -L../../ui/src -L../../rules/src -L../../multimedia/src \
	$(Plugin_TheoraVideoSystem_LIBS)

libRlScript_la_SOURCES = \
  ScriptSubsystem.cpp \
	../swig/RlExports.cxx 
