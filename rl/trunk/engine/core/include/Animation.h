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

#ifndef __rlAnimation_H__
#define __rlAnimation_H__

#include "CorePrerequisites.h"

#include <set>
#include <map>
#include "AnimationListener.h"


namespace rl {

class MeshObject;

/** 
	Diese Klasse ermöglicht eine einfache Steuerung von Animationseinstellungen
	und ist die Basisklasse erweiterter Animationen.
	@remarks Instanzen werden über den AnimationManager erzeugt
	@see AnimationManager
*/
class _RlCoreExport Animation : public virtual EventSource
{
    public:
		/**	Der Basiskonstruktor, für MeshObject, die einen AnimationState mitbringen
			@param animState	AnimationState, intern
			@param speed		Geschwindigkeit, auch negativ
			@param timesToPlay	Abspielanzahl, 0 = unendlich
            @mesh mesh  The MeshObjekt whose Animation is played
			@remarks	Die Animation beginnt sofort zu spielen, bei negativer
						Geschwindigkeit beginnt sie mit dem letzten Frame. Konstruktor
						sollte nicht direkt aufgerufen werden, sondern vom AnimationManager.
		*/
        Animation(Ogre::AnimationState* animState, MeshObject* mesh, Ogre::Real speed=1.0,
            unsigned int timesToPlay=0);
		/**	Ein Konstruktor, für eine später festlegbare Animation
		@remarks	Dieser Konstruktor ist für Unterklassen. Konstruktor
					sollte nicht direkt aufgerufen werden, sondern vom AnimationManager.
		*/
		Animation();

		/// Virtueller Destruktor
		virtual ~Animation( );
        
		/// Gibt zurück ob die Animation pausiert ist
        bool isPaused() const;
		/** Pausieren/Fortsetzen der Animation
			@param	isPaused	Zukünftiger Status
			@remarks	Löst einen AnimationPaused/Unpaused Event aus
		*/
        void setPaused( bool isPaused );

		/// Gibt zurück ob die globale Beschleunigung ignoriert wird
		bool isIgnoringGlobalSpeed() const;
		/**	Setzt die Ignoranz
			@param		isIgnoringGlobalSpeed Die zukünftige Ignoranz der globalen Geschwindigkeit
			@remarks	Möglichkeit die globale SlowMotion zu umgehen
						Nützlich für Statusanzeigen, oder ähnliche konstante Animationen
			@todo		TODO Eventuell das ganze auf Flags erweitern
		*/
		void setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed );

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

		/** Setzt die maximalen Wiederholungszahl der Animation
			@param	timesToPlay		Die nicht negative Anzahl der Wiederholungen
			@remarks	Bei 0 wird die Animation beliebig oft wiederholt
		*/
		void setTimesToPlay(unsigned int timesToPlay);
		/// Gibt die Anzahl der bereits vollständig abgespielten Wiederholungen zurück
		unsigned int getTimesPlayed() const;
		/** Setzt die Abspielzeit zurück
			Löst dabei auch eine mögliche Pause auf, und spult die Animation zurück
		*/
		void resetTimesPlayed();
		/// Gibt zurück wieviel Wiederholungen insgesamt abzuspielen sind
		unsigned int getTimesToPlay() const;
		/// Gibt zurück wieviele Wiederholungen noch durchzuführen sind
		unsigned int getTimesToPlayLeft() const;
		/// Gibt die Abspieldauer zurück (intern)
		Ogre::Real getTimePlayed() const;

		/// Gibt das Gewicht der Animation zurück
		Ogre::Real getWeight(void) const;
		/** Setzt das Gewicht (Einfluss) der Animation
			@param weigt Das Gewicht der Animation
		*/
        void setWeight( Ogre::Real weight );

        /// Gibt das MeshObject
        MeshObject* getMeshObject();

		/// Zeit hinzufügen - wird vom AnimationManager aufgerufen
		virtual void addTime( Ogre::Real timePassed );

		/** Fügt einen AnimationListener hinzu
			@param listener Der hinzuzufügende Listener
			@remarks Der Listener wird benachrichtigt, wenn
					  * die Animation pausiert/fortgesetzt wird
					  * die Animation ihr gesamten Wiederholungen vollendet hat
		*/
		void addAnimationListener( AnimationListener *listener);
		/// Entfernt einen AnimationListener
		void removeAnimationListener( AnimationListener *listener);
        /// Gibt den erstbesten Listener wieder (nur ne Testmethode)
        AnimationListener* getAnimationListener( );

		/** Fügt einen AnimationFrameListener hinzu
			@param listener Der hinzuzufügende Listener
			@param frameNumber Die zu überwachende Zeitindex
			@remarks	Der Listener wird benachrichtigt, wenn der Zeitindex erreicht oder 
						übersprungen wird. Dabei wird, falls der Fortschritt größer als die
						Länge der Animation korrekt geprüft, so dass keine Events verloren 
						gehen
		*/
		void addAnimationFrameListener( AnimationFrameListener *listener, Ogre::Real frameNumber );
		/// Entfernt einen AnimationListener an allen Zeitindizes
		void removeAnimationFrameListener( AnimationFrameListener *listener );
		/// Entfernt einen AnimationListener an einem bestimmtem Zeitindex
		void removeAnimationFrameListener( AnimationFrameListener *listener, Ogre::Real frameNumber );

		/// Gibt den AnimationState zurück (intern)
		Ogre::AnimationState* getAnimationState() const { return mAnimState; };
    protected:
		/// Der AnimationState
		Ogre::AnimationState* mAnimState;
        
        /// Das MeshObject
        MeshObject* mMeshObject;

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

		/// Setzt den AnimationState
		void setAnimationState( Ogre::AnimationState* animState );

		/// EventCaster
		EventCaster<AnimationEvent> mAnimationCaster;

        typedef std::multimap<Ogre::Real,AnimationFrameListener*> 
            AnimationFrameListenerMap;
		/// Die Multimap mit den FrameNummern und den dazugehörigen Listenern
		AnimationFrameListenerMap mAnimationFrameListener;
	private:
		/// Überwacht das erreichen der einzelnen Frames für die Listener
		void checkAnimationFrameListeners( Ogre::Real timePassed );
        void removeAllListeners();
};

}
#endif
