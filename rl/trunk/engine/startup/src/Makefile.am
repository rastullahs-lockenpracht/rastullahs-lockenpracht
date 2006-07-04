INCLUDES = $(CEGUI_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) \
	-I../../common/include -I../../core/include -I../../rules/include -I../../multimedia/include \
	-I../../ui/include -I../../dialog/include -I../../script/include -I../../ai/include

bin_PROGRAMS = rastullah
rastullah_SOURCES = RastullahMain.cpp
rastullah_LDFLAGS = -L../../script/src -L../../core/src  -L../../common/src -L../../dialog/src \
	-L../../ui/src -L../../rules/src -L../../multimedia/src -L../../uicomponents/src \
	-L../../ai/src
rastullah_LDADD = $(RUBY_LIBS) $(LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) \
	 $(Plugin_TheoraVideoSystem_LIBS) \
	 -lRlUiComponents -lRlCore -lRlScript -lRlCommon -lRlDialog -lRlUi -lRlRules -lRlMultimedia \
	 -logg -lvorbis -lboost_thread -lRlAi



