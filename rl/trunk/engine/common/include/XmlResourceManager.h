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

#ifndef __XmlResourceManager_H__
#define __XmlResourceManager_H__

#include "XmlResource.h"
#include "CommonPrerequisites.h"

namespace rl {

	class _RlCommonExport XmlResourceManager : 
		public Ogre::ResourceManager, public Ogre::Singleton<XmlResourceManager>
	{
	
    public:
        XmlResourceManager();
	protected:
        /// Eine Resource erzeugen
		Ogre::Resource* createImpl(const Ogre::String& resName, 
			Ogre::ResourceHandle handle, 
			const Ogre::String& groupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			bool isManual = false, Ogre::ManualResourceLoader* loader = NULL, const Ogre::NameValuePairList* loadParams = NULL);
	};

}

#endif
