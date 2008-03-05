/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

#include "stdinc.h" //precompiled header

#include "XmlResource.h"
#include "XmlResourceManager.h"
#include "XmlProcessor.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl {

XmlResource::XmlResource(
        ResourceManager* creator, const Ogre::String& name, ResourceHandle handle,
        const Ogre::String& group, bool isManual, ManualResourceLoader* loader)
	: Resource(creator, name, handle, group, isManual, loader),
	mCharBuffer(NULL),
	mXmlBuffer(NULL)
{
}


XmlResource::~XmlResource()
{
	unload();
}

void XmlResource::loadImpl()
{
	DataStreamPtr ds = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup);
	mSize = ds->size();
	
	mCharBuffer = new XMLByte[mSize];
	ds->read(mCharBuffer, mSize);

	mXmlBuffer = new MemBufInputSource(
        mCharBuffer,
	    static_cast<const unsigned int>(mSize), "rl::XmlResourceManager");
}

void XmlResource::unloadImpl()
{
	delete mXmlBuffer;
    mXmlBuffer = 0;
	delete[] mCharBuffer;
    mCharBuffer = 0;
}


size_t XmlResource::calculateSize() const
{
    return mSize;
}
/**
 * @todo both parseby methods could be merged in one template method, to avoid redundancy
 */
bool XmlResource::parseBy(XERCES_CPP_NAMESPACE::XercesDOMParser* parser, XmlProcessor* const errorHandler)
{
	if (!isLoaded())
		load();

    if (errorHandler 
        && parser->getErrorHandler() == NULL)
    {
        parser->setErrorHandler(errorHandler);
    }

    parser->parse(*mXmlBuffer);
    if(parser->getErrorCount() > 0)
    {
        return false;
    }
    return true;
}

bool XmlResource::parseBy(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser, XmlProcessor* const errorHandler)
{
	if (!isLoaded())
		load();

    if (errorHandler 
        && parser->getErrorHandler() == NULL)
    {
        parser->setErrorHandler(errorHandler);
    }

    parser->parse(*mXmlBuffer);
    if(parser->getErrorCount() > 0)
    {
        return false;
    }
    return true;
}

XmlPtr::XmlPtr(const ResourcePtr& res) : SharedPtr<XmlResource>()
{
    // lock & copy other mutex pointer
    OGRE_LOCK_MUTEX(*res.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(res.OGRE_AUTO_MUTEX_NAME)
    pRep = static_cast<XmlResource*>(res.getPointer());
    pUseCount = res.useCountPointer();
    if (pUseCount != 0)
        ++(*pUseCount);
}

XmlPtr& XmlPtr::operator =(const ResourcePtr& res)
{
    if (pRep == static_cast<XmlResource*>(res.getPointer()))
        return *this;
    release();

    // lock & copy other mutex pointer
    OGRE_LOCK_MUTEX(*res.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(res.OGRE_AUTO_MUTEX_NAME)
    pRep = static_cast<XmlResource*>(res.getPointer());
    pUseCount = res.useCountPointer();
    if (pUseCount != 0)
        ++(*pUseCount);

    return *this;
}

void XmlPtr::destroy()
{
    SharedPtr<XmlResource>::destroy();
}

}
