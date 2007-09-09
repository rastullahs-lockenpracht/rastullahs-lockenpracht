INCLUDES = $(CEGUI_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(OgreNewt_CFLAGS) $(OIS_CFLAGS) \
	$(STLPORT_CFLAGS) $(BOOST_CPPFLAGS) -I../include -I../../common/include \
	-I../../core/include -I../../rules/include -I../../ui/include \
	-I../../ai/include

lib_LTLIBRARIES = libRlScript.la
libRlScript_la_LDFLAGS = \
    $(top_builddir)/engine/common/src/libRlCommon.la \
    $(top_builddir)/engine/core/src/libRlCore.la \
    $(top_builddir)/engine/ui/src/libRlUi.la \
    $(top_builddir)/engine/rules/src/libRlRules.la \
    $(CEGUI_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS) \
    -Wl,--defsym -Wl,Init_libRlScript=Init_RlScript
	
libRlScript_la_SOURCES = \
	AbstractMapNodeProcessor.cpp \
	EntityNodeProcessor.cpp \
	EnvironmentProcessor.cpp \
	GameObjectNodeProcessor.cpp \
	LightNodeProcessor.cpp \
	MapLoader.cpp \
	ParticleSystemNodeProcessor.cpp \
	ScriptSubsystem.cpp \
	ScriptObjectMarker.cpp \
	SoundNodeProcessor.cpp \
	WaypointProcessor.cpp \
	ZoneProcessor.cpp \
	../swig/RlExports.cxx 
