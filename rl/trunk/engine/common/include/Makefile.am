noinst_HEADERS =
	CommonPrerequisites.h \
	ConfigFile.h \
	config.h \
	EventCaster.h \
	EventListener.h \
	EventObject.h \
	EventSource.h \
	Exception.h \
	FixRubyHeaders.h \
	GameTask.h \
	Logger.h \
	MathUtil.h \
	OgreXercesInput.h \
    Properties.h \
    Property.h \
    RastullahPrerequisites.h \
    ScriptWrapper.h \
	Sleep.h \
	Tripel.h \
	WriteableDataStream.h \
	WriteableDataStreamFormatTarget.h \
	WriteableFileSystemArchiv.h \
	XmlProcessor.h \
    XmlPropertyReader.h \
	XmlPropertyWriter.h \
	XmlResource.h \
	XmlResourceManager.h

CLEANFILES = stdinc.h.gch
all-local: $(noinst_HEADERS) stdinc.h
	@echo Creating GCH file
	g++ -I. stdinc.h 
