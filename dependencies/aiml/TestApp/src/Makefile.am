INCLUDES = -I../include -I../../MadaBot/include -I../../MadaBot/include/aimlProcessors -I../../MadaBot/include/botProcessors -I../../dependencies

bin_PROGRAMS = AimlTestApp
AimlTestApp_SOURCES = \
	main.cpp \
	SimpleXmlNodeImplXerces.cpp \
	XercesString.cpp \
	XmlParserImplXerces.cpp
AimlTestApp_LDFLAGS = -L../../MadaBot/src
AimlTestApp_LDADD =  $(LIBS) -lMadaBot 



