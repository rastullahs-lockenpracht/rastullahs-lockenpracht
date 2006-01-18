INCLUDES = $(RUBY_CFLAGS) $(CEGUI_CFLAGS) $(OGRE_CFLAGS) $(CEGUIOGRE_CFLAGS) $(STLPORT_CFLAGS) \
	$(Plugin_TheoraVideoSystem_CFLAGS) -I../../engine/sound/include -I../../engine/core/include -I../../engine/ui/include \
	-I../../engine/rules/include -I../../engine/dialog/include -I../../engine/script/include

noinst_PROGRAMS = testdialog
testdialog_LDFLAGS = -L../../engine/common/src -L../../engine/sound/src -L../../engine/core/src \
	-L../../engine/ui/src -L../../engine/rules/src -L../../engine/dialog/src \
	-L../../engine/rules/src -L../../engine/script/src -L../../engine/ui/src
testdialog_LDADD = $(RUBY_LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(CPPUNIT_LIBS) $(LIBS) \
	  -lRlCommon -lRlSound -lvorbis -logg $(Plugin_TheoraVideoSystem_LIBS) \
	  -lRlUi -lRlScript -lRlRules
testdialog_SOURCES = \
	TestDialog.cpp 

	