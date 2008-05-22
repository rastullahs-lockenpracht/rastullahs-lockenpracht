INCLUDES = $(CEGUI_CFLAGS) $(RUBY_CFLAGS) $(OPENALPP_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../../engine/rules/include -I../../engine/core/include

noinst_PROGRAMS = testrules
testrules_SOURCES = Main.cpp
testrules_LDFLAGS = -L../../engine/core/src  -L../../engine/common/src -L../../engine/rules/src
testrules_LDADD = $(LIBS) $(CPPUNIT_LIBS) $(OGRE_LIBS) $(CEGUIOGRE_LIBS) \
	 $(RUBY_LIBS) -lRlCore -lRlCommon -lRlRules -lvorbis -lvorbisfile -logg -lboost_thread \
	 $(Plugin_TheoraVideoSystem_LIBS)

