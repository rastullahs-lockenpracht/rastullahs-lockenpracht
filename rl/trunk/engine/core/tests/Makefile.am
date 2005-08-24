INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) -I../include \
 	  -I../../core/include -I../../common/include -I../../sound/include -I../../dialog/include  \
 	  -I../../ui/include -I../../rules/include -I../../script/include  $(CEGUI_CFLAGS)

noinst_PROGRAMS = testcore
testcore_LDFLAGS = -L../../common/src -L../../sound/src -L../../dialog/src -L../../rules/src \
	  -L../../ui/src -L../../core/src -L../../script/src
testcore_LDADD = $(RUBY_LIBS) $(CPPUNIT_LIBS) $(OGRE_LIBS) $(CEGUI_LIBS) $(LIBS) \
	  -lRlCommon -lRlCore -lRlSound -lRlUi -lRlRules -lRlDialog -lRlScript \
	  -lvorbis -logg -lboost_thread 
testcore_SOURCES = \
	TestCore.cpp \
	CoreTests.cpp

