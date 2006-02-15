INCLUDES = $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(CEGUIOGRE_CFLAGS) $(STLPORT_CFLAGS) \
	$(Plugin_TheoraVideoSystem_CFLAGS) -I../../engine/sound/include -I../../engine/core/include \
	-I../../engine/ui/include -I../../engine/dialog/include -I../../engine/rules/include \
	-I../../engine/script/include

noinst_PROGRAMS = testsound testvideo
testsound_LDFLAGS = -L../../engine/common/src -L../../engine/sound/src -L../../engine/core/src \
	-L../../engine/ui/src -L../../engine/rules/src -L../../engine/dialog/src \
	-L../../engine/script/src 
testsound_LDADD = $(RUBY_LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(CPPUNIT_LIBS) $(LIBS) \
	  -lRlCommon -lRlSound -lvorbis -logg $(Plugin_TheoraVideoSystem_LIBS) \
	  -lRlUi -lRlScript -lRlRules -lRlDialog -lRlCore
testsound_SOURCES = \
	SoundTest1.cpp \
	SoundTest2.cpp \
	SoundTest3.cpp \
	SoundTest4.cpp \
	SoundTest5.cpp \
	SoundTest6.cpp \
	SoundTest7.cpp \
	SoundTest8.cpp \
	TestStartup.cpp 

testvideo_LDFLAGS = -L../../engine/common/src -L../../engine/sound/src -L../../engine/core/src \
	-L../../engine/ui/src -L../../engine/rules/src -L../../engine/dialog/src
testvideo_SOURCES = \
	VideoTest.cpp
testvideo_LDADD = $(CEGUI_LIBS) $(OGRE_LIBS) $(LIBS) -lvorbis -logg -lboost_thread\
	$(Plugin_TheoraVideoSystem_LIBS)  $(CEGUIOGRE_LIBS) -lRlSound -lRlCommon -lRlCore \
	$(RUBY_LIBS)
	