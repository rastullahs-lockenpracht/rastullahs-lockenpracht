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

#include "XmlResourceManager.h"

using namespace Ogre;

template<> rl::XmlResourceManager* Singleton<rl::XmlResourceManager>::ms_Singleton = 0;

namespace rl {
// Gibt das Singleton zurueck.
XmlResourceManager& XmlResourceManager::getSingleton()
{
	return Ogre::Singleton<XmlResourceManager>::getSingleton();
}

// Gibt einen Zeiger auf das Singleton zurueck.
XmlResourceManager* XmlResourceManager::getSingletonPtr()
{
	return Ogre::Singleton<XmlResourceManager>::getSingletonPtr();
}

// Eine Resource erzeugen
XmlPtr XmlResourceManager::load(const String& resName, const String& groupName, bool isManual, 
	ManualResourceLoader* loader, const NameValuePairList* loadParams)
{
	XmlPtr xmlP = getByName(resName);
	
	if (xmlP.isNull())
		xmlP = ResourceManager::create(resName, groupName, isManual, loader, loadParams);

	return xmlP;
}

}
