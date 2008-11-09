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

#ifndef __ContentModule_H__
#define __ContentModule_H__

#include "CorePrerequisites.h"
#include "SaveGameData.h"
#include "ContentLoader.h"

namespace rl
{

    class _RlCoreExport ContentModule
	{
	public:
		ContentModule(const Ogre::String& id, const CeGuiString& name, bool common, long minimumEngineVersion);
		virtual ~ContentModule();

		const CeGuiString& getName() const;
		const Ogre::String& getId() const;
		const Ogre::String getDirectory() const;
		long getMinimumEngineVersion() const;
		bool isCommon() const;
			
		virtual const Ogre::StringVector& getDependencies() const = 0;
		virtual const Ogre::StringVector& getTextureLocations() const = 0;
		virtual const Ogre::StringVector& getSoundLocations() const = 0;
		virtual const Ogre::StringVector& getModelLocations() const = 0;
		virtual void start() = 0;

		static const Ogre::String getInitFile(const Ogre::String& moduleId);
		void initialize();
		void initializeTextures() const;
		void precreateMeshes() const;
        void loadContent();
		void unload();

        bool isLoaded() const;

        void registerContentLoader(ContentLoader* loader);
	private:
        typedef std::vector<ContentLoader*> ContentLoaderVector;
        ContentLoaderVector mContentLoaders;

		static const Ogre::String getDirectory(const Ogre::String& moduleId);
		void addSearchPath(const Ogre::String& path, const Ogre::String& resourceGroup) const;

		const Ogre::String mId;
		const CeGuiString mName;
		bool mCommon;
		long mMinimumEngineVersion;
        bool mLoaded;
	};

} //namespace rl

#endif
