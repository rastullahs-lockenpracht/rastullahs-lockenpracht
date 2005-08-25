INCLUDES = $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../core/include

noinst_PROGRAMS = testsound
testsound_LDFLAGS = -L../../common/src -L../../sound/src 
testsound_LDADD = $(RUBY_LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(CPPUNIT_LIBS) $(LIBS) \
	  -lRlCommon -lRlSound -lvorbis -logg -lboost_thread
testsound_SOURCES = \
	SoundSampleTest.cpp \
	SoundStreamTest.cpp \
	TestStartup.cpp 

