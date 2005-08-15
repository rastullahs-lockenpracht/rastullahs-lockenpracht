/* SoundSubSystem.cpp - Management von RlSound.
 * (C) 2003-2005. Team Pantheon. www.team-pantheon.de
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
#include "SoundSubsystem.h"
#include "SoundManager.h"
#include "Logger.h"

using namespace Ogre;

template<> rl::SoundSubsystem* Singleton<rl::SoundSubsystem>::ms_Singleton = 0;

namespace rl {

/**
 * @return Das Singleton-Objekt
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem& SoundSubsystem::getSingleton(void)
{
	return Singleton<SoundSubsystem>::getSingleton();
}

/**
 * @return Das Singleton-Zeiger
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem* SoundSubsystem::getSingletonPtr(void)
{
    return Singleton<SoundSubsystem>::getSingletonPtr();
}

/**
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem::SoundSubsystem()
{
    // fmod initialisieren und Fehler zuruecksetzen.
    FSOUND_SetMaxHardwareChannels(16);
    FSOUND_SetMinHardwareChannels(8);
    /// TODO: More choices
    //FSOUND_SetOutput(FSOUND_OUTPUT_ALSA);
    FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
    FSOUND_Init(44100, 32, 0); // TODO Wenns schiefgeht.
	log(Ogre::LML_TRIVIAL, "fmod initialisiert");
    
    // Wir initialisieren den Listener
    // Position of the listener.
    float v[3] = {0, 0, 0};
    FSOUND_3D_Listener_SetAttributes(v, v, 1, 0, 0, 1, 0, 0);
    log(Ogre::LML_TRIVIAL, "Listener set");
    
    //Singletons erzeugen 
    new SoundManager();

}

/**
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem::~SoundSubsystem()
{
    delete SoundManager::getSingletonPtr();
    FSOUND_Close();
}

/**
 * @author Blakharaz
 * @date 10-14-2004
 */
void SoundSubsystem::log(Ogre::LogMessageLevel level, const Ogre::String& msg, const Ogre::String& ident )
{
	Logger::getSingleton().log(level, "Sound", msg, ident);
}

/**
 * @author JoSch
 * @date 07-03-2005
 */
void SoundSubsystem::update(Real elapsedTime)
{
    setElapsedTime(elapsedTime);
    FSOUND_Update();
}

/**
 * @author JoSch
 * @date 07-24-2005
 */
Real SoundSubsystem::getElapsedTime() const
{
    return mElapsedTime;
}
/**
 * @author JoSch
 * @date 07-24-2005
 */
void SoundSubsystem::setElapsedTime(Real elapsedTime)
{
    mElapsedTime = elapsedTime;
}

}
