/* SoundSubSystem.cpp - Management von RlSound.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
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
#include "MusicManager.h"

extern "C" {
#include "AL/al.h"
#include "AL/alut.h"
}
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
    // OpenAL initialisieren und Fehler zuruecksetzen.
    alutInit(0, 0);
    alGetError();
    
    // Wir initialisieren den Listener
    // Position of the listener.
    ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
    // Velocity of the listener.
    ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
    // Orientation of the listener. (first 3 elements are "at", second 3 are "up")
    ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
    alListenerfv(AL_POSITION, ListenerPos);
    alListenerfv(AL_VELOCITY, ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
 
    
    //Singletons erzeugen
    new MusicManager();
    new SoundManager();
}

/**
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem::~SoundSubsystem()
{
    delete SoundManager::getSingletonPtr();
    delete MusicManager::getSingletonPtr();
    alutExit();
}

}
