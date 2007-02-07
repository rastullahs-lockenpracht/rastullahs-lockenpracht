/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "SoundManager.h"

#include <set>
#include "Actor.h"
#include "ActorManager.h"
#include "ConfigFile.h"
#include "GameLoop.h"
#include "ListenerMovable.h"
#include "ListenerObject.h"
#include "SoundDriver.h"
#include "SoundResource.h"
#include <OgreException.h>
#include <OgreResourceGroupManager.h>

#include "NullDriver.h"

using namespace std;
using namespace Ogre;

/** Das Singleton
 * @author JoSch
 * @date 04-26-2004
 */
template<> rl::SoundManager* Singleton<rl::SoundManager>::ms_Singleton = 0;

namespace rl {

    /**
     * @return Eine Referenz auf das Singleton-Objekt.
     * @author JoSch
     * @date 04-26-2004
     */
    SoundManager& SoundManager::getSingleton()
    {
        return Singleton<SoundManager>::getSingleton();
    }

    /**
     * @return Ein Zeiger auf das Singleton-Objekt.
     * @author JoSch
     * @date 04-26-2004
     */
    SoundManager* SoundManager::getSingletonPtr()
    {
        return Singleton<SoundManager>::getSingletonPtr();
    }

    /**
     * Standardkonstruktor
     * @author JoSch
     * @date 01-27-2005
     */
    SoundManager::SoundManager()
    : ResourceManager(),
      mDriverList(),
      mActiveDriver(NULL),
      mListenerActor(NULL),
      mActiveListener(NULL)
    {
        LOG_MESSAGE(Logger::CORE, "Erzeuge Soundmanager...");
        NullDriver* nullDriver = new NullDriver(this);
        registerDriver(nullDriver);
        setActiveDriver(nullDriver);

        mResourceType = "Sound";
        ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);

    }

    SoundManager::~SoundManager()
    {
        ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
        unloadAllDrivers();
    }

/**
 * Gibt die Suchmuster fuer die Extension zurueck.
 * @return Die Liste der Suchmuster
 * @author JoSch
 * @date 06-18-2004
 */
StringList SoundManager::getExtension()
{
    StringList result;
    result.push_back("*.ogg");
    result.push_back("*.wav");

    return result;
}

/**
* @param resName. Der Name der Soundresource, die erzeugt
* werden soll.
* @author JoSch
* @date 06-18-2004
*/
Resource* SoundManager::createImpl(
    const String& resName,
    ResourceHandle handle,
    const String& group,
    bool isManual,
    ManualResourceLoader* loader,
    const NameValuePairList* loadParams)
{
    SoundResource *newSound = NULL;
    newSound = new SoundResource(this, resName, handle, group, isManual, loader);
    return newSound;
}

/**
 * @param Der neue Treiber.
 * @author JoSch
 * @date 12-23-2005
 */
void SoundManager::setActiveDriver(SoundDriver *driver)
{
    if (driver == NULL)
    {
        driver = getDriverByName(NullDriver::NAME);
    }

    if (mActiveDriver == driver)
    {
        LOG_DEBUG(Logger::CORE,
            "No need to change Sound Driver, "
            + mActiveDriver->getName()
            + " is already active.");
        return;
    }

    LOG_MESSAGE(Logger::CORE,
        "Changing Sound Driver to "
         + driver->getName());

    if (driver->initialize())
    {
        if (mActiveDriver != NULL)
        {
            mActiveDriver->shutdown();
            delete mActiveDriver;
            mActiveDriver = NULL;
        }
        mActiveDriver = driver;
    }
    else
    {
        LOG_ERROR(
            Logger::CORE,
            "Sound driver "
            + driver->getName()
            + " had an error while initializing, keeping old driver.");
    }
}

void SoundManager::_clearListenerActor()
{
    mActiveListener = NULL;
    mListenerActor = NULL;
}


/**
 * @return Der aktive Treiber.
 * @author JoSch
 * @date 12-23-2005
 */
SoundDriver* SoundManager::getActiveDriver() const
{
    return mActiveDriver;
}

/**
 * @return Der aktive Listener.
 * @author JoSch
 * @date 04-04-2006
 */
ListenerObject* SoundManager::getListener() const
{
    return mActiveListener;
}

Actor* SoundManager::createListenerActor()
{
    if (mListenerActor == NULL)
    {
        if (mActiveListener != NULL)
        {
            mActiveListener->getListener()->setActive(false);
        }

        mListenerActor = ActorManager::getSingleton().createListenerActor("SoundListenerObject");
        mActiveListener = static_cast<ListenerObject*>(mListenerActor->getControlledObject());
        mActiveListener->getListener()->setActive(true);
        return mListenerActor;
    }
    else
    {
        Throw(IllegalStateException, "ListenerActor already created.");
    }
}

Actor* SoundManager::getListenerActor()
{
    return mListenerActor;
}

/**
 * Hole einen Treiber durch Angabe seines Namens
 * @param name Der name des gesuchten Treibers.
 * @return Der gesuchte Treiber
 * @author JoSch
 * @date 05-10-2006
 */
SoundDriver *SoundManager::getDriverByName(const String &name)
{
    DriverList::const_iterator it;
    for(it = mDriverList.begin(); it != mDriverList.end(); it++)
    {
        if ((*it)->getName() == name)
        {
            return *it;
        }
    }
    return 0;
}

    void SoundManager::registerDriver(rl::SoundDriver *driver)
    {
        mDriverList.push_back(driver);
    }

    void SoundManager::unregisterDriver(rl::SoundDriver *driver)
    {
        mDriverList.remove(driver);
    }

    void SoundManager::applySettings(const Ogre::NameValuePairList& settings)
    {
        // Get the sound driver
        Ogre::NameValuePairList::const_iterator it = settings.find("Driver");
        Ogre::String drivername;

        if (it == settings.end())
        {
            drivername = "RlFmod4Driver";
        }
        else
        {
            drivername = it->second;
        }

        try
        {
            #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Ogre::Root::getSingleton().loadPlugin(drivername);
            #else
            Ogre::Root::getSingleton().loadPlugin("lib" + drivername);
            #endif
        }
        catch(Ogre::Exception &e)
        {
            LOG_MESSAGE(Logger::CORE,
                CeGuiString("Soundtreiber kann nicht geladen werden: ")
                    + drivername + "\n"
                    + e.getFullDescription());
        }
        catch(...)
        {
            LOG_MESSAGE(Logger::CORE,
                 CeGuiString("Soundtreiber kann nicht geladen werden: ")
                    + drivername);
        }

        SoundDriver *driver = getDriverByName(drivername);

        if (driver == NULL)
        {
            driver = getDriverByName(NullDriver::NAME);
        }

        RlAssert(driver != NULL, "Beim Laden des Treibers ist ein Fehler aufgetreten");
        setActiveDriver(driver);
        getActiveDriver()->applySettings(settings);
    }

void SoundManager::unloadAllDrivers()
{
       if (mActiveDriver != NULL)
    {
        mActiveDriver->shutdown();
        mActiveDriver = NULL;
    }

    /**
     * @ToDo: This is a hack to avoid the problem with the Null driver.
     *        Don't know yet, why this happens.
     */
    DriverList::iterator it = mDriverList.begin();
    it++;
    /** Hack End **/
    for(/*DriverList::iterator it = mDriverList.begin()*/; it != mDriverList.end(); it++)
    {
#       if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        Ogre::String driverPlugin = (*it)->getName();
#       else
        Ogre::String driverPlugin = "lib" + (*it)->getName();
#       endif

        LOG_MESSAGE(Logger::CORE,
            "Unloading sound driver DLL "
            + driverPlugin);

        Ogre::Root::getSingleton().unloadPlugin(driverPlugin);

        LOG_MESSAGE(Logger::CORE,
            "Sound driver DLL "
            + driverPlugin
            + " successfully unloaded.");
    }

    mDriverList.clear();
}

/**
 * @author JoSch
 * @date 07-03-2005
 */
void SoundManager::run(Real elapsedTime)
{
    if (mActiveDriver != NULL)
    {
        mActiveDriver->update();
    }

    if (mListenerActor != NULL)
    {
        mListenerActor->_update();
    }
}

const Ogre::String& SoundManager::getName() const
{
    static String NAME = "SoundManager";

    return NAME;
}


}
