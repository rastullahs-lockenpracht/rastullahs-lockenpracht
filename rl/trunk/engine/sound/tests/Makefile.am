INCLUDES = $(RUBY_CFLAGS) $(SDL_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../core/include

noinst_PROGRAMS = testsound
testsound_LDFLAGS = -L../../common/src -L../../sound/src 
testsound_LDADD = $(RUBY_LIBS) $(SWIG_RUBY_LIBS) $(SDL_LIBS) $(OGRE_LIBS) $(LIBS) $(OGRE_PLUGINDIR)/Plugin_GuiElements.so \
	  -lRlCommon -lRlSound -lcppunit -lvorbis -lvorbisfile -logg -lboost_thread
testsound_SOURCES = \
	MusicManagerTest.cpp \
	SoundManagerTest.cpp \
	TestStartup.cpp 

