INCLUDES = $(CEGUI_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) $(STLPORT_CFLAGS) \
	$(BOOST_CPPFLAGS) -I../../common/include -I../../core/include -I../../rules/include \
	-I../../ui/include -I../../script/include -I../../ai/include $(OgreNewt_CFLAGS)

bin_PROGRAMS = rastullah
rastullah_SOURCES = RastullahMain.cpp
rastullah_LDADD = \
    $(top_builddir)/engine/common/src/libRlCommon.la \
    $(top_builddir)/engine/core/src/libRlCore.la \
    $(top_builddir)/engine/ui/src/libRlUi.la \
    $(top_builddir)/engine/rules/src/libRlRules.la \
    $(top_builddir)/engine/script/src/libRlScript.la \
    $(top_builddir)/engine/ai/src/libRlAi.la \
    $(RUBY_LIBS) $(LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(OgreNewt_LIBS)



