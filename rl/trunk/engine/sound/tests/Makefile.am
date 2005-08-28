INCLUDES = $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(CEGUIOGRE_CFLAGS) $(STLPORT_CFLAGS) \
	$(Plugin_TheoraVideoSystem_CFLAGS) -I../include -I../../core/include -I../../ui/include

noinst_PROGRAMS = testsound testvideo
testsound_LDFLAGS = -L../../common/src -L../../sound/src -L../../core/src \
	-L../../ui/src -L../../rules/src -L../../dialog/src
testsound_LDADD = $(RUBY_LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(CPPUNIT_LIBS) $(LIBS) \
	  -lRlCommon -lRlSound -lvorbis -logg
testsound_SOURCES = \
	SoundSampleTest.cpp \
	SoundStreamTest.cpp \
	TestStartup.cpp 

testvideo_LDFLAGS = -L../../common/src -L../../sound/src -L../../core/src \
	-L../../ui/src -L../../rules/src -L../../dialog/src
testvideo_SOURCES = \
	VideoTest.cpp
testvideo_LDADD = $(CEGUI_LIBS) $(OGRE_LIBS) $(LIBS) -lvorbis -logg -lboost_thread\
	$(Plugin_TheoraVideoSystem_LIBS)  $(CEGUIOGRE_LIBS) -lRlSound -lRlCommon