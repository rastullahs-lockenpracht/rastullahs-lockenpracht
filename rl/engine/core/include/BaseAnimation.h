/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#include <map>
#include <set>

namespace rl
{

    /**
        Diese Klasse ermoeglicht eine einfache Steuerung von Animationseinstellungen
        und ist die Basisklasse erweiterter Animationen.
        @remarks Instanzen werden ueber den AnimationManager erzeugt
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
        BaseAnimation(Ogre::Real length, Ogre::Real speed, unsigned int timesToPlay, bool paused);
        /**	Ein Konstruktor, fuer eine spaeter festlegbare Animation
         */
        BaseAnimation();

        /// Virtueller Destruktor
        virtual ~BaseAnimation();

        /// Gibt zurueck ob die Animation pausiert ist
        virtual bool isPaused() const;
        /** Pausieren/Fortsetzen der Animation
            @param	isPaused	Zukuenftiger Status
            @remarks	Loest einen AnimationPaused/Unpaused Event aus
        */
        virtual void setPaused(bool isPaused);

        /// Gibt zurueck ob die globale Beschleunigung ignoriert wird
        virtual bool isIgnoringGlobalSpeed() const;
        /**	Setzt die Ignoranz
            @param		isIgnoringGlobalSpeed Die zukuenftige Ignoranz der globalen Geschwindigkeit
            @remarks	Moeglichkeit die globale SlowMotion zu umgehen
                        Nuetzlich fuer Statusanzeigen, oder aehnliche konstante Animationen
        */
        virtual void setIgnoringGlobalSpeed(bool isIgnoringGlobalSpeed);

        /// Gibt die aktuelle Geschwindigkeit zurueck
        virtual Ogre::Real getSpeed() const;
        /**	Setzt die aktuelle Geschwindigkeit der Animation
            @param speed die Geschwindigkeit
            @remarks	1.0 ist die normale Geschwindigkeit der Animation
                        (mLength in Sekunden), negative Werte spielen die
                        Animation rueckwaerts ab. Bei 0 herrscht Stillstand.
        */
        virtual void setSpeed(Ogre::Real speed);
        /// Negiert die aktuelle Geschwindigkeit
        virtual void reverseAnimation();

        /** Setzt die maximalen Wiederholungszahl der Animation
            @param	timesToPlay		Die nicht negative Anzahl der Wiederholungen
            @remarks	Bei 0 wird die Animation beliebig oft wiederholt
        */
        virtual void setTimesToPlay(unsigned int timesToPlay);
        /// Gibt die Anzahl der bereits vollstaendig abgespielten Wiederholungen zurueck
        virtual unsigned int getTimesPlayed() const;
        /** Setzt die Abspielzeit zurueck
            Loest dabei auch eine moegliche Pause auf, und spult die Animation zurueck
        */
        virtual void resetTimesPlayed();
        /// Gibt zurueck wieviel Wiederholungen insgesamt abzuspielen sind
        virtual unsigned int getTimesToPlay() const;
        /// Gibt zurueck wieviele Wiederholungen noch durchzufuehren sind
        virtual unsigned int getTimesToPlayLeft() const;
        /// Gibt die Abspieldauer zurueck (intern)
        virtual Ogre::Real getTimePlayed() const;

        /** Setzt das Delay vor dem ersten Abspielen der Animation.
            @param delay Die Verzoegerung in Sekunden  */
        virtual void setDelay(Ogre::Real delay);
        /// Gibt die Verzoegerung vor dem Ersten Abspielen in Sekunden zurueck.
        virtual Ogre::Real getDelay() const;

        /// Zeit hinzufuegen - wird vom AnimationManager aufgerufen
        void addTime(Ogre::Real timePassed);

        /// addTime fuer alle Unterklassen
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
        virtual void setLoop(bool loop);
        /// Looping zurueckgeben
        virtual bool isLoop() const;
    };
}
#endif
