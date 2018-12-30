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

#include "SoundManager.h"

#include <set>

#include "Actor.h"
#include "ActorManager.h"
#include "ConfigFile.h"
#include "CoreSubsystem.h"
#include "GameLoop.h"
#include "ListenerMovable.h"
#include "ListenerObject.h"
#include "SoundDriver.h"
#include "SoundResource.h"

#ifdef USE_FMOD4
#include "Fmod4Driver.h"
#endif

#include "NullDriver.h"

using namespace std;
using namespace Ogre;

/** Das Singleton
 * @author JoSch
 * @date 04-26-2004
 */
template <> rl::SoundManager* Singleton<rl::SoundManager>::msSingleton = 0;

namespace rl
{
    /**
     * Standardkonstruktor
     * @author JoSch
     * @date 01-27-2005
     */
    SoundManager::SoundManager()
        : ResourceManager()
        , mDriverList()
        , mActiveDriver(NULL)
        , mListenerActor(NULL)
        , mActiveListener(NULL)
    {
        LOG_MESSAGE(Logger::MULTIMEDIA, "Erzeuge Soundmanager...");
        NullDriver* nullDriver = new NullDriver(this);
        registerDriver(nullDriver);
        setActiveDriver(nullDriver);

#ifdef USE_FMOD4
        Fmod4Driver* fmod4Driver = new Fmod4Driver(this);
        registerDriver(fmod4Driver);
#endif

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
    Resource* SoundManager::createImpl(const String& resName, ResourceHandle handle, const String& group, bool isManual,
        ManualResourceLoader* loader, const NameValuePairList* loadParams)
    {
        SoundResource* newSound = NULL;
        newSound = new SoundResource(this, resName, handle, group, isManual, loader);
        return newSound;
    }

    /**
     * @param Der neue Treiber.
     * @author JoSch
     * @date 12-23-2005
     */
    void SoundManager::setActiveDriver(SoundDriver* driver)
    {
        if (driver == NULL)
        {
            driver = getDriverByName(NullDriver::NAME);
        }

        if (mActiveDriver == driver)
        {
            LOG_DEBUG(Logger::MULTIMEDIA,
                "No need to change Sound Driver, " + mActiveDriver->getName() + " is already active.");
            return;
        }

        LOG_MESSAGE(Logger::MULTIMEDIA, "Changing Sound Driver to " + driver->getName());

        if (driver->initialize())
        {
            if (mActiveDriver != NULL)
            {
                mActiveDriver->shutdown();
                mActiveDriver = NULL;
            }
            mActiveDriver = driver;
        }
        else
        {
            LOG_ERROR(Logger::MULTIMEDIA,
                "Sound driver " + driver->getName() + " had an error while initializing, keeping old driver.");
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
    SoundDriver* SoundManager::getDriverByName(const String& name)
    {
        DriverList::const_iterator it;
        for (it = mDriverList.begin(); it != mDriverList.end(); it++)
        {
            if ((*it)->getName() == name)
            {
                return *it;
            }
        }
        return 0;
    }

    const std::list<SoundDriver*>& SoundManager::getDriverList()
    {
        return mDriverList;
    }

    void SoundManager::registerDriver(rl::SoundDriver* driver)
    {
        mDriverList.push_back(driver);
    }

    void SoundManager::unregisterDriver(rl::SoundDriver* driver)
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

        SoundDriver* driver = getDriverByName(drivername);

        if (driver == NULL)
        {
            try
            {
                CoreSubsystem::getSingleton().loadPlugin(drivername);
            }
            catch (Ogre::Exception& e)
            {
                LOG_MESSAGE(Logger::MULTIMEDIA,
                    CeGuiString("Soundtreiber kann nicht geladen werden: ") + drivername + "\n"
                        + e.getFullDescription());
            }
            catch (...)
            {
                LOG_MESSAGE(Logger::MULTIMEDIA, CeGuiString("Soundtreiber kann nicht geladen werden: ") + drivername);
            }
        }

        driver = getDriverByName(drivername);

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

        for (DriverList::iterator it = mDriverList.begin(); it != mDriverList.end(); it++)
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Ogre::String driverPlugin = (*it)->getName();
#else
            Ogre::String driverPlugin = "lib" + (*it)->getName();
#endif

            LOG_MESSAGE(Logger::MULTIMEDIA, "Unloading sound driver DLL " + driverPlugin);

            bool isDriverPlugin = (*it)->isDriverPlugin();
            if (isDriverPlugin)
            {
                Ogre::Root::getSingleton().unloadPlugin(driverPlugin);
            }
            else
            {
                delete *it;
            }

            LOG_MESSAGE(Logger::MULTIMEDIA, "Sound driver DLL " + driverPlugin + " successfully unloaded.");
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

    /**
     * @author JoSch
     * @date 20-04-2007
     * Delegate sound creation to active driver
     */
    Sound* SoundManager::createSound(const Ogre::String& res, SoundType type)
    {
        if (getActiveDriver() != NULL)
        {
            return getActiveDriver()->createSound(res, type);
        }
        return NULL;
    }

    /**
     * @author JoSch
     * @date 20-04-2007
     * Delegate sound creation to active driver
     */
    Sound* SoundManager::createSound(const SoundResourcePtr& res, SoundType type)
    {
        if (getActiveDriver() != NULL)
        {
            return getActiveDriver()->createSound(res, type);
        }
        return NULL;
    }

    /**
     * @author JoSch
     * @date 20-04-2007
     * Delegate sound destruction to active driver
     */
    void SoundManager::destroySound(Sound* sound)
    {
        if (getActiveDriver() != NULL)
        {
            getActiveDriver()->destroySound(sound);
        }
    }
}
