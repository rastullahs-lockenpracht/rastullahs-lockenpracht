INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../core/include

lib_LTLIBRARIES = libRlSound.la
libRlSound_la_LDFLAGS = $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS) 
libRlSound_la_SOURCES = \
	ListenerMovable.cpp \
	SoundChannel.cpp \
	SoundManager.cpp \
	Sound.cpp \
    SoundResource.cpp \
    SoundSample.cpp \
    SoundStream.cpp \
	SoundSubsystem.cpp \
	SoundEvents.cpp 
    