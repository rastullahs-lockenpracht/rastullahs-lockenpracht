INCLUDES = $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(RUBY_CFLAGS)  -I../include \
	-I../../../engine/ui/include -I../../../engine/core/include \
	-I../../../engine/common/include

lib_LTLIBRARIES = libRlOpenALDriver.la
libRlOpenALDriver_la_LDFLAGS = \
    $(top_builddir)/engine/common/src/libRlCommon.la \
    $(top_builddir)/engine/core/src/libRlCore.la \
    $(top_builddir)/engine/ui/src/libRlUi.la \
    -lopenal $(RUBY_LIBS) $(OGRE_LIBS) $(CEGUI_LIBS) $(LIBS)

libRlOpenALDriver_la_SOURCES = \
	OpenALConfigComponent.cpp \
	OpenALDriver.cpp \
	OpenALDriverDllMain.cpp \
	OpenALListener.cpp \
	OpenALSound.cpp \
	OpenALSoundSample.cpp \
	OpenALSoundStream.cpp
	
