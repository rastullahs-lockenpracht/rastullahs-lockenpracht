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

#include "ContentModule.h"

#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "RubyInterpreter.h"
#include "SaveGameManager.h"
#include "SoundManager.h"
#include "World.h"

using namespace Ogre;

namespace rl
{
    ContentModule::ContentModule(
        const Ogre::String& id, const CeGuiString& name, bool common, long minimumEngineVersion)
        : mId(id)
        , mName(name)
        , mCommon(common)
        , mMinimumEngineVersion(minimumEngineVersion)
        , mLoaded(false)
    {
    }

    ContentModule::~ContentModule()
    {
        for (ContentLoaderVector::const_iterator it = mContentLoaders.begin(); it != mContentLoaders.end(); ++it)
        {
            delete *it;
        }
        mContentLoaders.clear();
    }

    const Ogre::String ContentModule::getInitFile(const Ogre::String& moduleId)
    {
        return getDirectory(moduleId) + "/scripts/moduleconfig.rb";
    }

    const Ogre::String ContentModule::getDirectory(const Ogre::String& moduleId)
    {
        return ConfigurationManager::getSingleton().getModulesRootDirectory() + "/" + moduleId;
    }

    const Ogre::String ContentModule::getDirectory() const
    {
        return ContentModule::getDirectory(mId);
    }

    const CeGuiString& ContentModule::getName() const
    {
        return mName;
    }

    const Ogre::String& ContentModule::getId() const
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
        Ogre::String resourceGroup = getId();

        StringVector texLocations = getTextureLocations();
        for (StringVector::iterator iter = texLocations.begin(); iter != texLocations.end(); iter++)
        {
            Ogre::String location = *iter;
            if (location.find(".zip") != Ogre::String::npos)
            {
                ResourceGroupManager::getSingleton().addResourceLocation(
                    getDirectory() + "/materials/" + location, Ogre::String("Zip"), resourceGroup);
            }
            else
            {
                ResourceGroupManager::getSingleton().addResourceLocation(
                    getDirectory() + "/materials/" + location, "FileSystem", resourceGroup);
            }
        }

        addSearchPath(getDirectory() + "/materials", resourceGroup);
    }

    void ContentModule::initialize()
    {
        SaveGameManager::getSingleton().registerSaveGameData(this);

        Ogre::String moduleDir = getDirectory();

        Ogre::String resourceGroup = getId();

        addSearchPath(moduleDir + "/conf", resourceGroup);
        addSearchPath(moduleDir + "/dsa", resourceGroup);
        addSearchPath(moduleDir + "/maps", resourceGroup);
        addSearchPath(moduleDir + "/models", resourceGroup);
        StringVector modelLoc = getModelLocations();
        for (StringVector::iterator it = modelLoc.begin(); it != modelLoc.end(); ++it)
        {
            addSearchPath(moduleDir + "/models/" + *it, resourceGroup);
        }

        addSearchPath(moduleDir + "/sound", resourceGroup); ///@todo ueber Verzeichnisnamen nachdenken
        StringVector soundLoc = getSoundLocations();
        for (StringVector::iterator it = soundLoc.begin(); it != soundLoc.end(); ++it)
        {
            addSearchPath(moduleDir + "/sound/" + *it, resourceGroup);
        }

        addSearchPath(moduleDir + "/gui", resourceGroup);
        addSearchPath(moduleDir + "/gui/fonts", resourceGroup);
        addSearchPath(moduleDir + "/gui/imagesets", resourceGroup);
        addSearchPath(moduleDir + "/gui/schemes", resourceGroup);
        addSearchPath(moduleDir + "/gui/windows", resourceGroup);
        addSearchPath(moduleDir + "/gui/windows/buttons", resourceGroup);
        addSearchPath(moduleDir + "/dialogs", resourceGroup);
        addSearchPath(moduleDir + "/quests", resourceGroup);

        RubyInterpreter* interpreter = CoreSubsystem::getSingleton().getRubyInterpreter();
        if (interpreter != NULL)
        {
            interpreter->addSearchPath(moduleDir + "/scripts");
            interpreter->addSearchPath(moduleDir + "/scripts/maps");
        }

        mLoaded = true;
    }

    void ContentModule::addSearchPath(const Ogre::String& path, const Ogre::String& resourceGroup) const
    {
        try
        {
            ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", resourceGroup);
        }
        catch (...)
        {
            // and forget
        }
    }

    void ContentModule::precreateMeshes() const
    {
        StringVectorPtr meshes = ResourceGroupManager::getSingleton().findResourceNames(getId(), "*.mesh");

        for (size_t i = 0; i < meshes->size(); ++i)
        {
            ResourcePtr res = MeshManager::getSingleton().getByName((*meshes)[i]);
            if (!res)
            {
                MeshPtr mesh = MeshManager::getSingleton().create((*meshes)[i], getId());
            }
        }
    }

    void ContentModule::loadContent()
    {
        for (ContentLoaderVector::const_iterator it = mContentLoaders.begin(); it != mContentLoaders.end(); ++it)
        {
            (*it)->loadContent();
        }
    }

    void ContentModule::unload()
    {
        SaveGameManager::getSingleton().unregisterSaveGameData(this);
        for (ContentLoaderVector::const_iterator it = mContentLoaders.begin(); it != mContentLoaders.end(); ++it)
        {
            (*it)->unloadContent();
        }
        mContentLoaders.clear();
        // TODO: unloadModule
        CoreSubsystem::getSingleton().getWorld()->clearScene();
        mLoaded = false;
    }

    bool ContentModule::isLoaded() const
    {
        return mLoaded;
    }

    void ContentModule::registerContentLoader(ContentLoader* loader)
    {
        mContentLoaders.push_back(loader);
    }

    CeGuiString ContentModule::getXmlNodeIdentifier() const
    {
        return "ContentModule";
    }

    void ContentModule::writeData(SaveGameFileWriter* writer)
    {
        if (!this->isCommon())
        {
            LOG_MESSAGE(Logger::CORE, "Saving ContentLoaders");
            tinyxml2::XMLElement* contentLoadersNode
                = writer->appendChildElement(writer->getDocument()->RootElement(), getXmlNodeIdentifier().c_str());
            writer->setAttributeValueAsString(contentLoadersNode, "name", mName);

            for (ContentLoaderVector::const_iterator it = mContentLoaders.begin(); it != mContentLoaders.end(); ++it)
            {
                tinyxml2::XMLElement* contentLoaderNode
                    = writer->appendChildElement(contentLoadersNode, "contentloader");
                writer->setAttributeValueAsString(contentLoaderNode, "classname", Property((*it)->getClassName()));
                writer->writeEachPropertyToElem(contentLoaderNode, (*it)->getAllProperties()->toPropertyMap());
            }
        }
    }

    void ContentModule::readData(SaveGameFileReader* reader)
    {
    }

    /// defines the loading/saving order higher priority are saved last and loaded first
    int ContentModule::getPriority() const
    {
        return 10;
    }
}
