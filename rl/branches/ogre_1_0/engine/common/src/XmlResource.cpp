/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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
