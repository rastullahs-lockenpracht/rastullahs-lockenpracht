/* SoundSubsystem.h created 09.08.2005 by josch
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
#ifndef _SOUNDSUBSYSTEM_H_
#define _SOUNDSUBSYSTEM_H_

#include "SoundPrerequisites.h"
#include <OgreLogManager.h>
#include "Exception.h"

namespace rl
{

class _RlSoundExport SoundSubsystem : public Ogre::Singleton<SoundSubsystem>
{
public:
    // Konstruktor
	SoundSubsystem();
    // Destruktor
	virtual ~SoundSubsystem();
    
    /// Gibt Singleton-Objekt zurueck.
    static SoundSubsystem& getSingleton();
    /// Gibt Singleton-Zeiger zurueck.
    static SoundSubsystem* getSingletonPtr();
    /// Eine Log-Nachricht ausgeben.
    static void SoundSubsystem::log(const Ogre::String& msg);
    /// Wird EAX unterstützt?
    bool isEAXCapable() const;
    /// Interface zu EAXGet
    ALenum EAXGet(const void *propertySetID, ALuint property,
        ALuint source, ALvoid *value, ALuint size);
    /// Interface zu EAXSet
    ALenum EAXSet(const void *propertySetID, ALuint property,
        ALuint source, ALvoid *value, ALuint size);
    static void check() throw (RuntimeException);
};

};

#endif //_SOUNDSUBSYSTEM_H_
