INCLUDES = $(Plugin_TheoraVideoSystem_CFLAGS) $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) \
	 -I../include -I../../common/include -I../../core/include

lib_LTLIBRARIES = libRlSound.la
libRlSound_la_LIBADD = $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS) $(CEGUI_LIBS) $(CEGUIOGRE_LIBS) \
	$(Plugin_TheoraVideoSystem_LIBS) -lboost_thread -lRlCommon -lRlCore
libRlSound_la_LDFLAGS = -L../../core/src -L../../common/src

libRlSound_la_SOURCES = \
	ListenerMovable.cpp \
	SoundChannel.cpp \
	SoundManager.cpp \
	Sound.cpp \
    SoundResource.cpp \
    SoundSample.cpp \
    SoundStream.cpp \
	SoundSubsystem.cpp \
	SoundEvents.cpp \
	Video.cpp \
	VideoEvents.cpp 
    