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

#include "ContentModule.h"

#include <OgreMeshManager.h>
#include <OgreResource.h>

#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "RubyInterpreter.h"
#include "SoundManager.h"

using namespace Ogre;

namespace rl
{
	ContentModule::ContentModule(const String& id, const CeGuiString& name, bool common, long minimumEngineVersion)
	:	mId(id),
		mName(name),
		mCommon(common),
		mMinimumEngineVersion(minimumEngineVersion)
	{
	}

	ContentModule::~ContentModule()
	{
	}

	const String ContentModule::getInitFile(const String& moduleId)
	{
		return getDirectory(moduleId) + "/conf/moduleconfig.rb";
	}

	const String ContentModule::getDirectory(const String& moduleId)
	{
		return ConfigurationManager::getSingleton().
        		getModulesRootDirectory() + "/modules/" + moduleId;
	}

	const String ContentModule::getDirectory() const
	{
		return ContentModule::getDirectory(mId);
	}

	const CeGuiString& ContentModule::getName() const
	{
		return mName;
	}

	const String& ContentModule::getId() const
	{
		return mId;
	}

	long ContentModule::getMinimumEngineVersion() const
	{
		return mMinimumEngineVersion;
	}

	bool ContentModule::isCommon() const
	{
		return mCommon;
	}

    void ContentModule::initializeTextures() const
    {
        
		String resourceGroup = isCommon() ?
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME : getId();

		StringVector texLocations = getTextureLocations();
		for(StringVector::iterator iter = texLocations.begin();
			iter != texLocations.end();
			iter++)
		{
			String location = *iter;
			if (location.find(".zip") != String::npos)
			{
                ResourceGroupManager::getSingleton().addResourceLocation(
                	getDirectory() + "/materials/" + location, "Zip", resourceGroup);
			}
			else
			{
                ResourceGroupManager::getSingleton().addResourceLocation(
                	getDirectory() + "/materials/" + location, "FileSystem", resourceGroup);
			}
		}

		addSearchPath(getDirectory()+"/materials", resourceGroup);
    }

	void ContentModule::initialize() const
    {
        String moduleDir = getDirectory();

        String resourceGroup = isCommon() ?
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME : getId();

        addSearchPath(moduleDir + "/conf", resourceGroup);
        addSearchPath(moduleDir + "/dsa", resourceGroup);
        addSearchPath(moduleDir + "/maps", resourceGroup);
        addSearchPath(moduleDir + "/models", resourceGroup);
   		StringVector modelLoc = getModelLocations();
		for (StringVector::iterator it = modelLoc.begin(); it != modelLoc.end(); ++it)
		{
			addSearchPath(moduleDir + "/models/"+*it, resourceGroup);
		}

		addSearchPath(moduleDir + "/sound", resourceGroup); //@todo ueber Verzeichnisnamen nachdenken
		StringVector soundLoc = getSoundLocations();
		for (StringVector::iterator it = soundLoc.begin(); it != soundLoc.end(); ++it)
		{
			addSearchPath(moduleDir + "/sound/"+*it, resourceGroup);
		}

		addSearchPath(moduleDir + "/gui", resourceGroup);
        addSearchPath(moduleDir + "/gui/fonts", resourceGroup);
        addSearchPath(moduleDir + "/gui/imagesets", resourceGroup);
        addSearchPath(moduleDir + "/gui/schemes", resourceGroup);
        addSearchPath(moduleDir + "/gui/windows", resourceGroup);
        addSearchPath(moduleDir + "/gui/windows/buttons", resourceGroup);
        addSearchPath(moduleDir + "/dialogs", resourceGroup);     

		SoundManager::getSingleton().addSounds( resourceGroup );


		RubyInterpreter* interpreter = CoreSubsystem::getSingleton().getRubyInterpreter();
        if (interpreter != NULL)
        {
            interpreter->addSearchPath(moduleDir + "/scripts");
            interpreter->addSearchPath(moduleDir + "/scripts/maps");
        }
    }

	void ContentModule::addSearchPath(const Ogre::String& path, const Ogre::String& resourceGroup) const
    {
        try 
        {
            ResourceGroupManager::getSingleton().addResourceLocation(path, 
                "FileSystem", resourceGroup);
        } 
        catch(...) 
        {
			// and forget
		}
    }

	void ContentModule::precreateMeshes() const
    {
        StringVectorPtr meshes = ResourceGroupManager::getSingleton()
            .findResourceNames(getId(), "*.mesh");

        for (size_t i = 0; i < meshes->size(); ++i)
        {
            ResourcePtr res = MeshManager::getSingleton().getByName((*meshes)[i]);
            if (res.isNull())
            {
                MeshPtr mesh = MeshManager::getSingleton().create((*meshes)[i],
                    getId());
            }
        }
    }

	void ContentModule::unload() const
    {
        //TODO: unloadModule
    }
}
