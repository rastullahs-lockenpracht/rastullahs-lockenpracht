INCLUDES = $(CEGUI_CFLAGS) $(RUBY_CFLAGS) $(OPENALPP_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../core/include

noinst_PROGRAMS = testrules
testrules_SOURCES = Main.cpp
testrules_LDFLAGS = -L../../core/src  -L../../common/src -L../../rules/src
testrules_LDADD = $(LIBS) $(OPENALPP_LIBS) $(CPPUNIT_LIBS) $(OGRE_LIBS) \
	 $(RUBY_LIBS) -lRlCore -lRlCommon -lRlRules -lvorbis -lvorbisfile -logg -lboost_thread

