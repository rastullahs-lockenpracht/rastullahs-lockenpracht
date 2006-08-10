INCLUDES = $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(RUBY_CFLAGS) -I../include \
	-I../../../engine/ui/include -I../../../engine/core/include \
	-I../../../engine/common/include

lib_LTLIBRARIES = libRlFmod3Driver.la
libRlFmod3Driver_la_LDFLAGS = $(RUBY_LIBS) $(OGRE_LIBS) $(CEGUIOGRE_LIBS) $(LIBS) \
	-L../../../engine/common/src -L../../../engine/core/src -L../../../engine/ui/src \
	-lRlCommon -lRlCore -lRlUi -lruby1.8
libRlFmod3Driver_la_SOURCES = \
	Fmod3Config.cpp \
	Fmod3Driver.cpp \
	Fmod3DriverDllMain.cpp \
	Fmod3Listener.cpp \
	Fmod3Sound.cpp \
	Fmod3SoundSample.cpp \
	Fmod3SoundStream.cpp
	