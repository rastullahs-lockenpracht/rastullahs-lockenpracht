INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) $(CEGUI_CFLAGS) -I../include -I../../core/include

lib_LTLIBRARIES = libRlCommon.la
libRlCommon_la_LDFLAGS = $(RUBY_LIBS) $(OGRE_LIBS) $(CEGUI_LIBS) $(BOOST_FILESYSTEM_LIB) $(LIBS)
libRlCommon_la_SOURCES = \
  ConfigFile.cpp \
  EventObject.cpp \
  EventSource.cpp \
  GameTask.cpp \
  Logger.cpp \
  LinkedPlaylist.cpp \
  MathUtil.cpp \
  OgreXercesInput.cpp \
  PauseObject.cpp \
  Playlist.cpp \
  PlaylistEvent.cpp \
  PlaylistObject.cpp \
  Properties.cpp \
  Property.cpp \
  PropertyReader.cpp \
  ScriptWrapper.cpp \
  SimplePlaylist.cpp \
  XmlErrorHandler.cpp \
  XmlHelper.cpp \
  XmlResource.cpp \
  XmlResourceManager.cpp   
