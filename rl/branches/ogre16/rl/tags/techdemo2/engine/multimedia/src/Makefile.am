SUBDIRS =

INCLUDES = $(Plugin_TheoraVideoSystem_CFLAGS) $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) \
	 -I../include -I../../common/include -I../../core/include -I../include/null

lib_LTLIBRARIES = libRlMultimedia.la
libRlMultimedia_la_LIBADD = $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS) $(CEGUI_LIBS) $(CEGUIOGRE_LIBS) \
	$(Plugin_TheoraVideoSystem_LIBS) -lboost_thread -lRlCommon $(VORBISFILE_LIBS)
libRlMultimedia_la_LDFLAGS = -L../../common/src

libRlMultimedia_la_SOURCES = \
	ListenerMovable.cpp \
	Sound.cpp \
	SoundChannel.cpp \
	SoundDriver.cpp \
	SoundEvents.cpp \
	SoundResource.cpp \
	VorbisFile.cpp \
	null/NullSoundChannel.cpp \
	null/NullSound.cpp \
	null/NullDriver.cpp \
	null/NullListener.cpp
