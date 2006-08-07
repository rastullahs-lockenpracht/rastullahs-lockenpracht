INCLUDES = $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(RUBY_CFLAGS) -I../include \
	-I../../../engine/ui/include -I../../../engine/core/include -I../../../engine/multimedia/include \
	-I../../../engine/common/include

lib_LTLIBRARIES = libRlFmod4Driver.la
libRlFmod4Driver_la_LDFLAGS = $(OGRE_LIBS) $(CEGUIOGRE_LIBS) $(LIBS) \
	-L../../../engine/common/src -L../../../engine/core/src -L../../../engine/ui/src \
	-L../../../engine/multimedia/src \
	-lRlCommon -lRlCore -lRlMultimedia -lRlUi
libRlFmod4Driver_la_SOURCES = \
	Fmod4Config.cpp \
	Fmod4Driver.cpp \
	Fmod4DriverDllMain.cpp \
	Fmod4Listener.cpp \
	Fmod4SoundChannel.cpp \
	Fmod4SoundSample.cpp \
	Fmod4SoundStream.cpp
	