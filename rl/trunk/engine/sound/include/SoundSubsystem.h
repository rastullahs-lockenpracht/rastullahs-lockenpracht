/* Copyright 2004 Jochen Sch?fer */
#ifndef __SOUNDSUBSYSTEM_H__
#define __SOUNDSUBSYSTEM_H__

#include "SoundPrerequisites.h"

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
};

}

#endif
