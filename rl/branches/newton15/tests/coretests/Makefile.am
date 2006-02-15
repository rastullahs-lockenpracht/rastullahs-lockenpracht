INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include \
 	  -I../../engine/core/include -I../../engine/common/include -I../../engine/sound/include -I../../engine/dialog/include  \
 	  -I../../engine/ui/include -I../../engine/rules/include -I../../engine/script/include  $(CEGUI_CFLAGS)

noinst_PROGRAMS = testcore
testcore_LDFLAGS = -L../../engine/common/src -L../../engine/sound/src -L../../engine/dialog/src -L../../engine/rules/src \
	  -L../../engine/ui/src -L../../engine/core/src -L../../engine/script/src
testcore_LDADD = $(RUBY_LIBS) $(CPPUNIT_LIBS) $(OGRE_LIBS) $(CEGUI_LIBS) $(LIBS) \
	  -lRlCommon -lRlCore -lRlSound -lRlUi -lRlRules -lRlDialog -lRlScript \
	  -lvorbis -logg -lboost_thread $(Plugin_TheoraVideoSystem_LIBS)
testcore_SOURCES = \
	TestCore.cpp \
	CoreTests.cpp

