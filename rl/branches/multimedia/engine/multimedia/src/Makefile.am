INCLUDES = $(Plugin_TheoraVideoSystem_CFLAGS) $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) \
	 -I../include -I../../common/include -I../../core/include -I../include/null

lib_LTLIBRARIES = libRlMultimedia.la
libRlMultimedia_la_LIBADD = $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS) $(CEGUI_LIBS) $(CEGUIOGRE_LIBS) \
	$(Plugin_TheoraVideoSystem_LIBS) -lboost_thread -lRlCommon
libRlMultimedia_la_LDFLAGS = -L../../common/src

libRlMultimedia_la_SOURCES = \
	ListenerMovable.cpp \
	MultimediaSubsystem.cpp \
	SoundManager.cpp \
	Sound.cpp \
	SoundChannel.cpp \
	SoundDriver.cpp \
    SoundResource.cpp \
	SoundEvents.cpp \
	SoundUpdateTask.cpp \
	Video.cpp \
	VideoEvents.cpp \
	null/NullSoundChannel.cpp \
	null/NullSoundSample.cpp \
	null/NullSoundStream.cpp \
	null/NullDriver.cpp

if with_fmod
INCLUDES += $(FMOD_CFLAGS) -I../include/fmod -DWITH_FMOD
libRlMultimedia_la_LIBADD += $(FMOD_LIBS)
libRlMultimedia_la_SOURCES += \
	fmod/FmodSoundChannel.cpp \
	fmod/FmodDriver.cpp \
	fmod/FmodSoundSample.cpp \
	fmod/FmodSoundStream.cpp
endif
	
if with_oal
INCLUDES += $(OPENAL_CFLAGS) -I../include/openal -DWITH_FMOD
libRlMultimedia_la_LIBADD += $(OPENAL_LIBS)
libRlMultimedia_la_SOURCES += \
	openal/OalSoundChannel.cpp \
	openal/OalDriver.cpp \
	openal/OalSoundSample.cpp \
	openal/OalSoundStream.cpp
endif
