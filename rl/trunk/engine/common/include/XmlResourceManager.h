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

#ifndef __XmlResourceManager_H__
#define __XmlResourceManager_H__

#include "XmlResource.h"
#include "CommonPrerequisites.h"

namespace rl {

	class _RlCommonExport XmlResourceManager : 
		public Ogre::ResourceManager, public Ogre::Singleton<XmlResourceManager>
	{
	
    public:
        /// Gibt das Singleton zurueck.
        static XmlResourceManager& getSingleton();
        /// Gibt einen Zeiger auf das Singleton zurueck.
        static XmlResourceManager* getSingletonPtr();
        /// Eine Resource erzeugen
        XmlResource* create(const String& resName);
	};

}

#endif
