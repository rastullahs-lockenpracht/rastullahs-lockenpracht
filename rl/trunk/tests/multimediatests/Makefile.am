INCLUDES = $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(CEGUIOGRE_CFLAGS) $(STLPORT_CFLAGS) \
	$(Plugin_TheoraVideoSystem_CFLAGS) -I../../engine/sound/include -I../../engine/core/include -I../../engine/ui/include

noinst_PROGRAMS = testsound testvideo
testsound_LDFLAGS = -L../../engine/common/src -L../../engine/sound/src -L../../engine/core/src \
	-L../../engine/ui/src -L../../engine/rules/src -L../../engine/dialog/src
testsound_LDADD = $(RUBY_LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(CPPUNIT_LIBS) $(LIBS) \
	  -lRlCommon -lRlSound -lvorbis -logg $(Plugin_TheoraVideoSystem_LIBS)
testsound_SOURCES = \
	SoundSampleTest.cpp \
	SoundStreamTest.cpp \
	TestStartup.cpp 

testvideo_LDFLAGS = -L../../engine/common/src -L../../engine/sound/src -L../../engine/core/src \
	-L../../engine/ui/src -L../../engine/rules/src -L../../engine/dialog/src
testvideo_SOURCES = \
	VideoTest.cpp
testvideo_LDADD = $(CEGUI_LIBS) $(OGRE_LIBS) $(LIBS) -lvorbis -logg -lboost_thread\
	$(Plugin_TheoraVideoSystem_LIBS)  $(CEGUIOGRE_LIBS) -lRlSound -lRlCommon -lRlCore \
	$(RUBY_LIBS)
	