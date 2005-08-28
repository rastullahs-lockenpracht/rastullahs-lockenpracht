INCLUDES = $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(CEGUIOGRE_CFLAGS) $(STLPORT_CFLAGS) \
	$(Plugin_TheoraVideoSystem_CFLAGS) -I../include -I../../core/include

noinst_PROGRAMS = testsound
testsound_LDFLAGS = -L../../common/src -L../../sound/src -L../../core/src
testsound_LDADD = $(RUBY_LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(CPPUNIT_LIBS) $(LIBS) \
      $(Plugin_TheoraVideoSystem_LIBS)  $(CEGUIOGRE_LIBS) \
	  -lRlCommon -lRlSound -lvorbis -logg -lboost_thread \
	  -lRlCore
testsound_SOURCES = \
	SoundSampleTest.cpp \
	SoundStreamTest.cpp \
	TestStartup.cpp  \
	VideoTest.cpp

