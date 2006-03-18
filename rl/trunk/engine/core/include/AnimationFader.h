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

#ifndef __AnimationFader_H__
#define __AnimationFader_H__

#include "CorePrerequisites.h"

namespace rl {

class Animation;
class AnimationFadeListener;

/** Eine Klasse zum automatischen Überblenden zwischen zwei (oder mehr) Animationen.
  * Zur Zeit nur lineares Überblenden. 
  * @todo Möglicherweise mit Animation verschmelzen oder selbe Basis geben...
  * Jede Animation benötigt - FadeAnfang, FadeEnde
  */
class _RlCoreExport AnimationFader
{
public:
	// Kon-/Destruktor
	AnimationFader();
	virtual ~AnimationFader();

	/// Fügt einen FadeListener hinzu
	void addAnimationFadeListener(AnimationFadeListener* afl);
	/// Entfernt einen bestimmten FadeListener
	void removeAnimationFadeListener(AnimationFadeListener* afl);
	/// Entfernt alle FadeListener
	void removeAllAnimationFadeListeners();



	/// Gibt die aktuelle Geschwindigkeit zurück
	Ogre::Real getSpeed() const;
	/**	Setzt die aktuelle Geschwindigkeit der Animation
		@param speed die Geschwindigkeit
		@remarks	1.0 ist die normale Geschwindigkeit der Animation 
					(mLength in Sekunden), negative Werte spielen die 
					Animation rückwärts ab. Bei 0 herrscht Stillstand.
	*/
	void setSpeed( Ogre::Real speed );
	/// Negiert die aktuelle Geschwindigkeit
	void reverseAnimation();

	/// Gibt zurück ob die globale Beschleunigung ignoriert wird
	bool isIgnoringGlobalSpeed() const;
	/**	Setzt die Ignoranz
		@param		isIgnoringGlobalSpeed Die zukünftige Ignoranz der globalen Geschwindigkeit
		@remarks	Möglichkeit die globale SlowMotion zu umgehen
					Nützlich für Statusanzeigen, oder ähnliche konstante Animationen
	*/
	void setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed );

	/// Gibt zurück ob dieser Fader nach Beendigung automatisch gelöscht wird
	bool isDeleteOnFinish() const;
	/**	Löscht diesen Fader nach Beendigung automatisch */
	void setDeleteOnFinish( bool deleteOnFinish );
private:
	/// Geschwindigkeit
	Ogre::Real mSpeed;
	/// Ignoriert die globale Geschwindigkeit
	bool mIgnoringGlobalSpeed;
	/// Löschen wenn das Faden beendet ist
	bool mDeleteOnFinish;	
};

}
#endif
