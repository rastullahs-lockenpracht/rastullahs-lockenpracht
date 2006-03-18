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
#include "MultimediaSubsystem.h"
#include "SoundManager.h"
#include "Logger.h"
#include "Video.h"
#include <stdio.h>
#include "SoundUpdateTask.h"
#include "SoundDriver.h"
#include "NullDriver.h"
#ifdef WITH_FMOD3
#include "Fmod3Driver.h"
#endif
#ifdef WITH_OAL
#include "OalDriver.h"
#endif

using namespace Ogre;

template<> rl::MultimediaSubsystem* Singleton<rl::MultimediaSubsystem>::ms_Singleton = 0;

namespace rl {

/**
 * @return Das Singleton-Objekt
 * @author JoSch
 * @date 05-26-2004
 */
MultimediaSubsystem& MultimediaSubsystem::getSingleton(void)
{
	return Singleton<MultimediaSubsystem>::getSingleton();
}

/**
 * @return Das Singleton-Zeiger
 * @author JoSch
 * @date 05-26-2004
 */
MultimediaSubsystem* MultimediaSubsystem::getSingletonPtr(void)
{
    return Singleton<MultimediaSubsystem>::getSingletonPtr();
}

/**
 * @author JoSch
 * @date 05-26-2004
 */
MultimediaSubsystem::MultimediaSubsystem():
	mActiveDriver(0),
	mElapsedTime(0)
{   
    SoundDriver *driver = NULL;
    // Die Treiberliste ermitteln.
    mDriverList.clear();
    // Immer Nulltreiber
    SoundDriver *nullDriver = new NullDriver();
    mDriverList.push_back(nullDriver);
#ifdef WITH_FMOD3
    // Fmod testen.
    driver = new Fmod3Driver();
    if (driver->isDriverAvailable())
    {
        mDriverList.push_back(driver);
    } else {
        delete driver;
    }
    setActiveDriver(nullDriver);
#else
    // Nulltreiber als aktiven Treiber setzen.
    setActiveDriver(nullDriver);
#endif
#ifdef WITH_OAL
    // OpenAL testen.
    driver = new OalDriver();
    if (driver->isDriverAvailable())
    {
        mDriverList.push_back(driver);
    } else {
        delete driver;
    }
#endif
     
  	//Singletons erzeugen 
   	new SoundManager();
    
    // SoundUpdates anschmeissen.	
    new SoundUpdateTask();
}

/**
 * @author JoSch
 * @date 05-26-2004
 */
MultimediaSubsystem::~MultimediaSubsystem()
{
    delete SoundManager::getSingletonPtr();
    delete SoundUpdateTask::getSingletonPtr();
    if (getActiveDriver() != 0)
    {
        getActiveDriver()->deInit();
    }
}

/**
 * @author Blakharaz
 * @date 10-14-2004
 */
void MultimediaSubsystem::log(Ogre::LogMessageLevel level, const Ogre::String& msg, const Ogre::String& ident )
{
    if (Logger::getSingletonPtr() != 0)
    {
        Logger::getSingleton().log("Multimedia", level, msg, ident);
    }
}

/**
 * @author JoSch
 * @date 07-03-2005
 */
void MultimediaSubsystem::update(Real elapsedTime)
{
    setElapsedTime(elapsedTime);
    // TODO FSOUND_Update();
    for(VideoList::iterator i = mVideoList.begin(); // TODO: The STL way
        i != mVideoList.end(); i++)
    {
        (*i)->update();
    }
}

/**
 * @author JoSch
 * @date 07-24-2005
 */
Real MultimediaSubsystem::getElapsedTime() const
{
    return mElapsedTime;
}

/**
 * @author JoSch
 * @date 07-24-2005
 */
void MultimediaSubsystem::setElapsedTime(Real elapsedTime)
{
    mElapsedTime = elapsedTime;
}

/**
 * @param video Das Video, das hinzugefügt werden soll.
 * @author JoSch
 * @date 09-06-2005
 */
void MultimediaSubsystem::addVideo(Video *video)
{
    mVideoList.push_back(video);
}

/**
 * @return Alle Videos in der Liste löschen
 * @author JoSch
 * @date 09-06-2005
 */
void MultimediaSubsystem::clearVideos()
{
    mVideoList.clear();
}

/**
 * @param Das Video, das gelöscht werden soll
 * @author JoSch
 * @date 09-06-2005
 */
void MultimediaSubsystem::removeVideo(Video *video)
{
    for(VideoList::iterator i = mVideoList.begin();
        i != mVideoList.end();)
    {
        if (*i == video)
        {
            i = mVideoList.erase(i);
        } else {
            i ++;
        }
    }
}

/**
 * @return Die Liste der funktionierenden Treiber.
 * @author JoSch
 * @date 12-23-2005
 */
const DriverList& MultimediaSubsystem::getSoundDriverList() const
{
    return mDriverList;
}

/**
 * @param Der neue Treiber.
 * @author JoSch
 * @date 12-23-2005
 */
void MultimediaSubsystem::setActiveDriver(SoundDriver *driver)
{
    if (mActiveDriver != 0 && driver != 0)
    {
        MultimediaSubsystem::log(LML_TRIVIAL, (CeGuiString("Soundtreiber wird gewechselt von ")
            + mActiveDriver->getName() + CeGuiString(" zu ") + driver->getName()).c_str());
    } else if (mActiveDriver != 0)
    {
        MultimediaSubsystem::log(LML_TRIVIAL, (CeGuiString("Soundtreiber wird gewechselt von ")
            + mActiveDriver->getName()).c_str());
    } else {
        MultimediaSubsystem::log(LML_TRIVIAL, (CeGuiString("Soundtreiber wird gewechselt zu ")
             + driver->getName()).c_str());
    }
    mActiveDriver = driver;
    if (mActiveDriver != 0)
    {
        mActiveDriver->init();
    }
}

/**
 * @return Der aktive Treiber.
 * @author JoSch
 * @date 12-23-2005
 */
SoundDriver* MultimediaSubsystem::getActiveDriver() const
{
    return mActiveDriver;
}


}
