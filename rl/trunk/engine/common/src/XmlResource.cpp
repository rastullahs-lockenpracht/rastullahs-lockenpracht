#include "XmlResource.h"
#include "XmlResourceManager.h"

using namespace XERCES_CPP_NAMESPACE;

namespace rl {

XmlResource::XmlResource(const Ogre::String& name)
{
	mName = name;
	mIsLoaded = false;
}


XmlResource::~XmlResource()
{
	unload();
}

void XmlResource::load()
{
	DataChunk dc;
	XmlResourceManager::getSingleton()._findResourceData(mName, dc);
	mXmlBuffer = new MemBufInputSource(dc.getPtr(), static_cast<const unsigned int>(dc.getSize()), "rl::XmlResourceManager");
	mIsLoaded = true;
	touch();
}

void XmlResource::unload()
{
	delete mXmlBuffer;
	mIsLoaded = false;
}

void XmlResource::parseBy(XERCES_CPP_NAMESPACE::XercesDOMParser* parser)
{
	if (!mIsLoaded)
		load();
	parser->parse(*mXmlBuffer);
	touch();
}

void XmlResource::parseBy(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser)
{
	if (!mIsLoaded)
		load();
	parser->parse(*mXmlBuffer);
	touch();
}

}
