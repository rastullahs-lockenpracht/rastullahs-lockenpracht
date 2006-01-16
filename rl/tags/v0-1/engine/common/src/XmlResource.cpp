/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "XmlResource.h"
#include "XmlResourceManager.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl {

XmlResource::XmlResource(
	 ResourceManager* creator, const String& name, ResourceHandle handle,
		const String& group, bool isManual, ManualResourceLoader* loader)
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
	
	mCharBuffer = new char[mSize];
	ds->read(mCharBuffer, mSize);

	mXmlBuffer = new MemBufInputSource(reinterpret_cast<XMLByte*>(mCharBuffer),
	    static_cast<const unsigned int>(mSize), "rl::XmlResourceManager");
	mIsLoaded = true;
	touch();	
}

void XmlResource::unloadImpl()
{
	delete mXmlBuffer;
	delete[] mCharBuffer;
	mIsLoaded = false;
}


size_t XmlResource::calculateSize() const
{
    return mSize;
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
