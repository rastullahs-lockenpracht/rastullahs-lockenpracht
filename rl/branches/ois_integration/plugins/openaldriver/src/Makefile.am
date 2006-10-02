INCLUDES = $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(RUBY_CFLAGS)  -I../include \
	-I../../../engine/ui/include -I../../../engine/core/include \
	-I../../../engine/common/include

lib_LTLIBRARIES = libRlOpenALDriver.la
libRlOpenALDriver_la_LDFLAGS = $(RUBY_LIBS) $(OGRE_LIBS) $(CEGUI_LIBS) $(LIBS) \
	-L../../../engine/common/src -L../../../engine/core/src -L../../../engine/ui/src \
	-lRlCommon -lRlCore -lRlUi -lruby1.8
libRlOpenALDriver_la_SOURCES = \
	OpenALDriver.cpp \
	OpenALConfig.cpp \
	OpenALDriverDllMain.cpp \
	OpenALListener.cpp \
	OpenALSound.cpp \
	OpenALSoundSample.cpp \
	OpenALSoundStream.cpp
	