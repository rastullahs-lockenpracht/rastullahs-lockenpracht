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
	SoundDriver *driver = NULL;
    mDriverList.clear();
	NullDriver* nullDriver = new NullDriver(this);
    registerDriver(nullDriver);
    setActiveDriver(nullDriver);
}

SoundManager::~SoundManager()
{
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

ResourcePtr SoundManager::create(const String& name, const String& group, 
            bool isManual, ManualResourceLoader* loader , 
            const NameValuePairList* createParams)
{
    ResourcePtr p = ResourceManager::create(name, group, isManual, loader, createParams);
    return p;
}

/**
 * @author JoSch
 * @date 04-27-2004
 */
void SoundManager::addSounds(const Ogre::String& group)
{
    StringList extlist = getExtension();
    StringList::const_iterator cit;
    for(cit = extlist.begin(); cit != extlist.end(); cit++)
    {
		StringVectorPtr sl = ResourceGroupManager::getSingleton().findResourceNames(
			group, *cit );
        StringVector::const_iterator it;
        for(it = sl->begin(); it != sl->end(); it++)
        {
            try {
                create(*it, group);
            } catch(...)
            {}
        }
    }
}

void SoundManager::add(const String& filename, const String& group)
{
	boost::mutex::scoped_lock lock(mResListMutex);
    create(filename, group);
}

/**
 * Erzeugt eine Liste von Soundnamen.
 * @return Erzeugte Namensliste.
 * @author JoSch
 * @date 06-17-2004
 */
StringList SoundManager::getSounds()
{
    StringList result;
    ResourceMapIterator it = getResourceIterator();
    while (it.hasMoreElements())
    {
        result.push_back(it.peekNextValue()->getName());
        it.moveNext();
    }
    
    return result;
}

/**
 * @return Die Liste der funktionierenden Treiber.
 * @author JoSch
 * @date 12-23-2005
 */
const DriverList& SoundManager::getSoundDriverList() const
{
    return mDriverList;
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

/**
 * Hole die Soundkonfiguration
 * 
 * @author JoSch
 * @date 05-08-2006
 */
void SoundManager::loadConf(const Ogre::String &filename)
{
	ConfigFile conf;
	conf.load(filename);
	
	mDrivers = conf.getValues("Driver", "Drivers");
	for (StringVector::const_iterator it = mDrivers.begin(); 
		it != mDrivers.end(); it++)
	{
        try {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		    Ogre::Root::getSingleton().loadPlugin(*it);
#else
            Ogre::Root::getSingleton().loadPlugin("lib"+*it);
#endif
        }
        catch(Ogre::Exception &e)
        {
            LOG_MESSAGE(Logger::CORE,
                CeGuiString("Soundtreiber kann nicht geladen werden: ")
                    + *it + "\n"
                    + e.getFullDescription());
        }
        catch(...)
        {
            LOG_MESSAGE(Logger::CORE,
                CeGuiString("Soundtreiber kann nicht geladen werden: ")
                    + *it);
        }
            
	}

	String drivername = conf.getValue(NullDriver::NAME, "ActiveDriver", "General");
	SoundDriver *driver = getDriverByName(drivername);
    if (driver == NULL)
    {
        driver = getDriverByName(NullDriver::NAME);
    }
	RlAssert(driver != NULL, "Beim Laden des Treibers ist ein Fehler aufgetreten");
	setActiveDriver(driver);
	getActiveDriver()->loadConf(conf);
}

/**
 * Schreibe die Soundkonfiguration
 * @author JoSch
 * @date 05-08-2006
 * @param filename Der Dateiname der Konfiguration
 */
void SoundManager::saveConf(const Ogre::String &filename) const
{
	RlAssert(getActiveDriver() != NULL, "Kein aktiver Soundtreiber");
	ConfigFile conf;
	conf.setValues(mDrivers, "Driver", "Drivers");
	conf.setValue(String(getActiveDriver()->getName().c_str()), "ActiveDriver", "General");
	getActiveDriver()->saveConf(conf);
	conf.save(filename);
}

void SoundManager::unloadAllDrivers()
{
   	if (mActiveDriver != NULL)
    {
        mActiveDriver->shutdown();
        mActiveDriver = NULL;
    }

	for(StringVector::iterator it = mDrivers.begin();
		it != mDrivers.end(); it++)
	{
		Ogre::String driverPlugin = *it;

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
