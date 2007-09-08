INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(STLPORT_CFLAGS) $(CEGUI_CFLAGS) -I../include -I../../core/include

lib_LTLIBRARIES = libRlCommon.la
libRlCommon_la_LDFLAGS = $(RUBY_LIBS) $(OGRE_LIBS) $(CEGUI_LIBS) $(BOOST_FILESYSTEM_LIB) $(LIBS)
libRlCommon_la_SOURCES = \
  ../include/stdinc.h \
  ConfigFile.cpp \
  EventObject.cpp \
  EventSource.cpp \
  GameTask.cpp \
  Logger.cpp \
  MathUtil.cpp \
  OgreXercesInput.cpp \
  Properties.cpp \
  Property.cpp \
  ScriptWrapper.cpp \
  XmlErrorHandler.cpp \
  XmlProcessor.cpp \
  XmlPropertyReader.cpp \
  XmlPropertyWriter.cpp \
  XmlResource.cpp \
  XmlResourceManager.cpp   
