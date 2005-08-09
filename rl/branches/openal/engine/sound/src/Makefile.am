INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include -I../../core/include

lib_LTLIBRARIES = libRlSound.la
libRlSound_la_LDFLAGS =$(SDL_LIBS) $(OGRE_LIBS) $(RUBY_LIBS) $(LIBS)
libRlSound_la_SOURCES = \
	SoundSubsystem.cpp
    