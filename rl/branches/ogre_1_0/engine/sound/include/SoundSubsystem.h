/* SoundSubSystem.h - Management von RlSound.
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

#ifndef __SOUNDSUBSYSTEM_H__
#define __SOUNDSUBSYSTEM_H__

#include "SoundPrerequisites.h"
#include <OgreLogManager.h>

namespace rl {

/** Diese Klasse dient der Initialisierung und Steuerung des
 * Sound-Subsystems.
 * @author Josch
 * @date 05-26-2004
 * @version 1.0
 */
    class _RlSoundExport SoundSubsystem : public Ogre::Singleton<SoundSubsystem>
{
public:
	/// Der Standardkonstruktor
	SoundSubsystem();
	/// Der Destruktor
    ~SoundSubsystem();

	/// Gibt Singleton-Objekt zurueck.
    static SoundSubsystem& getSingleton(void);
    /// Gibt Singleton-Zeiger zurueck.
    static SoundSubsystem* getSingletonPtr(void);
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
    /// Ist das Soundsystem schon blockiert?
    //bool isLocked() const;
};

}

#endif
