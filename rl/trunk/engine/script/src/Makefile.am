INCLUDES = $(CEGUI_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) \
	$(STLPORT_CFLAGS) -I../include -I../../common/include -I../../core/include \
	-I../../dialog/include -I../../rules/include -I../../ui/include \
	-I../../ai/include

lib_LTLIBRARIES = libRlScript.la
libRlScript_la_LDFLAGS = \
    $(top_builddir)/engine/common/src/libRlCommon.la \
    $(top_builddir)/engine/core/src/libRlCore.la \
    $(top_builddir)/engine/dialog/src/libRlDialog.la \
    $(top_builddir)/engine/ui/src/libRlUi.la \
    $(top_builddir)/engine/rules/src/libRlRules.la \
    $(CEGUI_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS) \ 
    -Wl,--defsym -Wl,Init_libRlScript=Init_RlScript
	
libRlScript_la_SOURCES = \
  ScriptSubsystem.cpp \
  ScriptObjectMarker.cpp \
	../swig/RlExports.cxx 
