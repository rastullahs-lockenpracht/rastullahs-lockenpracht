INCLUDES = $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(RUBY_CFLAGS) -I../include \
	-I../../../engine/ui/include -I../../../engine/core/include \
	-I../../../engine/common/include

lib_LTLIBRARIES = libRlFmod4Driver.la
libRlFmod4Driver_la_LDFLAGS = \
     $(top_builddir)/engine/common/src/libRlCommon.la \
     $(top_builddir)/engine/core/src/libRlCore.la \
     $(top_builddir)/engine/ui/src/libRlUi.la \
     -lfmodex $(OGRE_LIBS) $(CEGUIOGRE_LIBS) $(LIBS)

libRlFmod4Driver_la_SOURCES = \
	Fmod4Config.cpp \
	Fmod4Driver.cpp \
	Fmod4DriverDllMain.cpp \
	Fmod4Listener.cpp \
	Fmod4Sound.cpp
	
