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
     Log* log = LogManager::getSingleton().createLog( "logs/rlSound.log" );
     if (log != 0)
     {
        log->setLogDetail( LL_BOREME );
     }
 
    // fmod initialisieren und Fehler zuruecksetzen.
    FSOUND_SetMaxHardwareChannels(16);
    FSOUND_SetMinHardwareChannels(8);
    /// TODO: More choices
    //FSOUND_SetOutput(FSOUND_OUTPUT_ALSA);
    FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
    FSOUND_Init(44100, 32, 0); // TODO Wenns schiefgeht.
    SoundSubsystem::log("fmod initialisiert");
    
    // Wir initialisieren den Listener
    // Position of the listener.
    float v[3] = {0, 0, 0};
    FSOUND_3D_Listener_SetAttributes(v, v, 1, 0, 0, 1, 0, 0);
    SoundSubsystem::log("Listener set");
    
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
void SoundSubsystem::log(const String& msg)
{
    if (LogManager::getSingletonPtr() == 0)
    {
        new LogManager();
    }
    if (LogManager::getSingletonPtr() != 0)
    {
        LogManager::getSingleton().getLog( "logs/rlSound.log" )->logMessage(msg);
    }
}

/**
 * @author JoSch
 * @date 07-03-2005
 */
void SoundSubsystem::update()
{
    FSOUND_Update();
}

}
