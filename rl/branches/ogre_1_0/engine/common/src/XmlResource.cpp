/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include "XmlResource.h"
#include "XmlResourceManager.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

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

void XmlResource::loadImpl()
{
	DataStreamPtr ds = Ogre::ResourceGroupManager::getSingleton().openResource(getName(), getGroup());
	std::string inputData = ds.getPointer()->getAsString();
	mSize = ds.getPointer()->size();
	mXmlBuffer = new MemBufInputSource(reinterpret_cast<const XMLByte*>(inputData.c_str()), inputData.length(), "rl::XmlResourceManager");
	mIsLoaded = true;
	touch();
}

void XmlResource::unloadImpl()
{
	delete mXmlBuffer;
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
