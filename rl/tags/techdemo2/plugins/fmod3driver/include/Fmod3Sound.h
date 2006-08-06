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
#ifndef FMOD3SOUND_H_
#define FMOD3SOUND_H_

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
        Fmod3Sound(const SoundResourcePtr &soundres) : Sound(soundres), mChannel(NO_CHANNEL) {};
		/// Erzeuge einen Kanal mit Fmod.
		virtual int createChannel() throw (RuntimeException) = 0;

	protected:
		int mChannel;

		static const int NO_CHANNEL = -999999;
};

}

#endif /* FMOD3SOUND_H_ */
