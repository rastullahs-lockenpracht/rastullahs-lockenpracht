/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __BASEANIMATION_H__
#define __BASEANIMATION_H__

#include "CorePrerequisites.h"

#include <set>
#include <map>

namespace rl {

/** 
	Diese Klasse ermöglicht eine einfache Steuerung von Animationseinstellungen
	und ist die Basisklasse erweiterter Animationen.
	@remarks Instanzen werden über den AnimationManager erzeugt
	@see AnimationManager
*/
class _RlCoreExport BaseAnimation
{
    public:
		/**
            @param speed		Geschwindigkeit, auch negativ
			@param timesToPlay	Abspielanzahl, 0 = unendlich
            @param paused       Startet pausiert wenn true
		*/
        BaseAnimation(Ogre::Real length, Ogre::Real speed, unsigned int timesToPlay, bool paused );
		/**	Ein Konstruktor, für eine später festlegbare Animation
  		 */
		BaseAnimation();

		/// Virtueller Destruktor
		virtual ~BaseAnimation();
        
		/// Gibt zurück ob die Animation pausiert ist
        virtual bool isPaused() const;
		/** Pausieren/Fortsetzen der Animation
			@param	isPaused	Zukünftiger Status
			@remarks	Löst einen AnimationPaused/Unpaused Event aus
		*/
        virtual void setPaused(bool isPaused);

		/// Gibt zurück ob die globale Beschleunigung ignoriert wird
		virtual bool isIgnoringGlobalSpeed() const;
		/**	Setzt die Ignoranz
			@param		isIgnoringGlobalSpeed Die zukünftige Ignoranz der globalen Geschwindigkeit
			@remarks	Möglichkeit die globale SlowMotion zu umgehen
						Nützlich für Statusanzeigen, oder ähnliche konstante Animationen
		*/
		virtual void setIgnoringGlobalSpeed(bool isIgnoringGlobalSpeed);

		/// Gibt die aktuelle Geschwindigkeit zurück
		virtual Ogre::Real getSpeed() const;
		/**	Setzt die aktuelle Geschwindigkeit der Animation
			@param speed die Geschwindigkeit
			@remarks	1.0 ist die normale Geschwindigkeit der Animation 
						(mLength in Sekunden), negative Werte spielen die 
						Animation rückwärts ab. Bei 0 herrscht Stillstand.
		*/
		virtual void setSpeed(Ogre::Real speed);
		/// Negiert die aktuelle Geschwindigkeit
		virtual void reverseAnimation();

		/** Setzt die maximalen Wiederholungszahl der Animation
			@param	timesToPlay		Die nicht negative Anzahl der Wiederholungen
			@remarks	Bei 0 wird die Animation beliebig oft wiederholt
		*/
		virtual void setTimesToPlay(unsigned int timesToPlay);
		/// Gibt die Anzahl der bereits vollständig abgespielten Wiederholungen zurück
		virtual unsigned int getTimesPlayed() const;
		/** Setzt die Abspielzeit zurück
			Löst dabei auch eine mögliche Pause auf, und spult die Animation zurück
		*/
		virtual void resetTimesPlayed();
		/// Gibt zurück wieviel Wiederholungen insgesamt abzuspielen sind
		virtual unsigned int getTimesToPlay() const;
		/// Gibt zurück wieviele Wiederholungen noch durchzuführen sind
		virtual unsigned int getTimesToPlayLeft() const;
		/// Gibt die Abspieldauer zurück (intern)
		virtual Ogre::Real getTimePlayed() const;

		/** Setzt das Delay vor dem ersten Abspielen der Animation.
		    @param delay Die Verzögerung in Sekunden  */
		virtual void setDelay(Ogre::Real delay);
		/// Gibt die Verzögerung vor dem Ersten Abspielen in Sekunden zurück.
		virtual Ogre::Real getDelay() const;

		/// Zeit hinzufügen - wird vom AnimationManager aufgerufen
		void addTime(Ogre::Real timePassed);

        /// addTime für alle Unterklassen
		virtual void doAddTime(Ogre::Real timePassed) = 0;

        /// Starte die Animation
        virtual void start();
        /// Stoppe die Animation
        virtual void stop();
        /// Pausiere die Animation
        virtual void pause();

        virtual Ogre::Real getLength() const;
    protected:
		/// Ein Delay vor dem ersten Abspielen
		Ogre::Real mDelay;
        /// Dauer
        Ogre::Real mLength;
		/// Pause
        bool mPaused;
		/// Ignoriert die globale Geschwindigkeit
		bool mIgnoringGlobalSpeed;
		/// Eigene Geschwindigkeit
		Ogre::Real mSpeed;
		/// Gesamte Abspielwiederholungen
		unsigned int mTimesToPlay;
		/// Bisherige Abspielzeit
		Ogre::Real mTimePlayed;

        /// Looping setzen
		virtual void setLoop( bool loop );
		/// Looping zurückgeben
		virtual bool isLoop() const;
};

}
#endif
