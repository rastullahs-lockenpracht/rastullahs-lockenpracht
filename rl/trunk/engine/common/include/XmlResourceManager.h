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
