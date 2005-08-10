/* SoundSubsystem.cpp created 09.08.2005 by josch
 * This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

using namespace Ogre; 

// Das eigentliche Singleton-Objekt
template<> rl::SoundSubsystem* Singleton<rl::SoundSubsystem>::ms_Singleton = 0;

namespace rl
{

/**
 * @author JoSch
 * @date 08-09-2005
 */
SoundSubsystem::SoundSubsystem()
{
     Log* log = LogManager::getSingleton().createLog( "logs/rlSound.log" );
     if (log != 0)
     {
        log->setLogDetail( LL_BOREME );
     }
 
    // OpenAL initialisieren und Fehler zuruecksetzen.
    alutInit(0, 0);
    SoundSubsystem::log("Error " + StringConverter::toString(alGetError()));
    SoundSubsystem::log("AL initialised");
}

/**
 * @author JoSch
 * @date 08-09-2005
 */
SoundSubsystem::~SoundSubsystem()
{
    alutExit();
    SoundSubsystem::log("AL closed");
}

/**
 * @return Das Singleton-Objekt
 * @author JoSch
 * @date 08-09-2005
 */
SoundSubsystem& SoundSubsystem::getSingleton()
{
    return Singleton<SoundSubsystem>::getSingleton();
}

/**
 * @return Das Singleton-Zeiger
 * @author JoSch
 * @date 08-09-2005
 */
SoundSubsystem* SoundSubsystem::getSingletonPtr()
{
    return Singleton<SoundSubsystem>::getSingletonPtr();
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
 * @date 01-26-2005
 */
bool SoundSubsystem::isEAXCapable() const
{
    return alIsExtensionPresent((ALubyte*)"EAX2.0") == AL_TRUE;
}

/**
 * @author JoSch
 * @date 01-26-2005
 */
void SoundSubsystem::check() throw (RuntimeException)
{
    ALuint error = alGetError();
} 


/**
 * @author JoSch
 * @date 01-26-2005
 */
/*bool SoundSubsystem::isLocked() const
{
    return false;
} */

typedef ALenum (*funcEAX)(const void *propertySetID, ALuint property,
        ALuint source, ALvoid *value, ALuint size);
        
/**
 * @return Error code
 * @param propertySetID Zeiger auf set GUID
 * @param property Eigenschaft
 * @param source Source ID
 * @param value Zeiger auf den Wert
 * @param size Die Groesse des Werts
 * @author JoSch
 * @date 01-26-2005
 */
ALenum SoundSubsystem::EAXGet(const void *propertySetID, ALuint property,
        ALuint source, ALvoid *value, ALuint size)
{
    if (isEAXCapable())
    {
        funcEAX get = (funcEAX) alGetProcAddress((ALubyte*)"EAXGet");
        return (get)(propertySetID, property, source, value, size); 
    } else {
        return AL_FALSE;
    }
}

/**
 * @return Error code
 * @param propertySetID Zeiger auf set GUID
 * @param property Eigenschaft
 * @param source Source ID
 * @param value Zeiger auf den Wert
 * @param size Die Groesse des Werts
 * @author JoSch
 * @date 01-26-2005
 */
ALenum SoundSubsystem::EAXSet(const void *propertySetID, ALuint property,
        ALuint source, ALvoid *value, ALuint size)
{
    if (isEAXCapable())
    {
        funcEAX set = (funcEAX) alGetProcAddress((ALubyte*)"EAXSet");
        return (set)(propertySetID, property, source, value, size); 
    } else {
        return AL_FALSE;
    }
}



}; // End of namespace
