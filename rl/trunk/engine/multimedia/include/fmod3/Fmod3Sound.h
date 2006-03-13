#ifndef FMOD3SOUND_H_
#define FMOD3SOUND_H_

#ifdef WITH_FMOD3

#include "Sound.h"

namespace rl {

class SoundChannel; 

/** Diese Klasse sollte eigentlich leer sein, aber C++ verlangt
 * Konstruktoren, wenn in der Basisklasse kein Defaultkonstruktor
 * vorhanden ist. Ich durchbreche daher die festgelegten
 * Implementierungsregeln.
 * @author JoSch
 * @date 03-07-06
 */
class Fmod3Sound : public Sound
{
	public:
		/// Konstruktor
        Fmod3Sound(const Ogre::String& name) : Sound(name) {}
        /// Konstruktor
        Fmod3Sound(const SoundResourcePtr &soundres) : Sound(soundres) {};
		/// Erzeuge einen Kanal mit Fmod.
		virtual int createChannel() throw (RuntimeException) = 0;
};

}

#endif /* WITH_FMOD3 */
#endif /*FMOD3SOUND_H_*/
