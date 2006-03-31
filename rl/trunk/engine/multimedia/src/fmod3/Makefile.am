if with_fmod3
INCLUDES = $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) \
	 -I../../include -I../../../common/include -I../../../core/include $(FMOD3_CFLAGS)\
	 -I../../include/fmod3 -DWITH_FMOD3
lib_LTLIBRARIES = libRlFmod3.la
libRlFmod3_la_LIBADD = $(FMOD_LIBS)
libRlFmod3_la_SOURCES = \
	Fmod3SoundChannel.cpp \
	Fmod3Driver.cpp \
	Fmod3SoundSample.cpp \
	Fmod3SoundStream.cpp
endif
