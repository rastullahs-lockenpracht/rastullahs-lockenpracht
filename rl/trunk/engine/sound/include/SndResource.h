/* Copyright 2004 Jochen Sch?fer */
#ifndef SNDRESOURCE_H
#define SNDRESOURCE_H

#include "SoundPrerequisites.h"
#include "Ogre.h"
#include "boost/thread/thread.hpp"
#include "boost/thread/mutex.hpp"

using namespace boost;

// @TODO: Callbacks für Threads einfuehren.

namespace rl {
 
 
/** Diese Basisklasse kapselt eine OpenAl++-Source fuer
 * den ResourceManager von Ogre
 * @author Josch
 * @date 06-19-2004
 * @date 07-23-2004
 * @version 2.0
 */
class _RlSoundExport SndResource: public Resource {
    private:
        /// Diese Klasse kapselt das Fade in.
        class FadeOperation {
            private:
                /// Damit wir wissen, wo wir hinwollen ;-)
                SndResource *that;
                /// Fade In oder Out
                bool mFadeIn;
            public:
                /// Der Konstruktor.
                FadeOperation(SndResource *that, bool fadeIn);
                /// Die Arbeitsroutine.
                void operator()();

                /// Wie lange soll der Fade laufen (in msek.)?
                unsigned int mDuration;
        } mFadeInOperation, mFadeOutOperation;
       friend class FadeOperation;
        
    
    protected:
        // Die Fade-Threads
        thread *mFadeInThread, *mFadeOutThread;
        /// Die gekapselte Soundquelle
        ALuint mSource;
        /// Die Buffer, die wir benutzen
        ALuint *mBuffers;
        /// Wieviele Buffer werden benutzt.
        short mBufferCount;

        /// Ueberpruefen, ob Fehler aufgetreten ist.
        void check() const throw (RuntimeException);
        
        /// Mutex zum Synchronisieren von Gain-Zugriffen.
        mutable boost::mutex mGainMutex;
        
        /// Fuehre das Fade-In aus
        void fadeIn(unsigned int msec);
        /// Fuehre das Fade-Out aus
        void fadeOut(unsigned int msec);
        // Soll gefadet werden?
        bool mShouldFadeIn, mShouldFadeOut;
        
        /// Berechne den Anstieg der Lautstarke beim Fade-In
        ALfloat calculateFadeIn(unsigned RL_LONGLONG duration, unsigned RL_LONGLONG time,
                ALfloat gain);

        /// Berechne die Abnahme der Lautstarke beim Fade-Out
        ALfloat calculateFadeOut(unsigned RL_LONGLONG duration, unsigned RL_LONGLONG time,
                ALfloat gain);

    public:
        /// Der Standardkonstruktor
        SndResource(const Ogre::String& name);
        /// Der Destruktor
        virtual ~SndResource();
        /// Gibt die eingestellte Position der Soundquelle zurueck
        const Ogre::Vector3 getPosition() const throw (RuntimeException);
        /// Setzt die Position der Soundquelle.
        void setPosition(const Ogre::Vector3& direction) throw (RuntimeException);
        /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
        const ALfloat getGain() const throw (RuntimeException);
        /// Setzt die relative Lautstaerke der Soundquelle.
        void setGain(const ALfloat gain) throw (RuntimeException);
        /// Gibt die Richtung der Soundquelle zurueck.
        const Ogre::Vector3 getDirection() const throw (RuntimeException);
        /// Gibt die Geschwindigkeit der Soundquelle zurueck.
        const Ogre::Vector3 getVelocity() const throw (RuntimeException);
        /// Setzt die Richtung der Soundquelle.
        void setDirection(const Ogre::Vector3&) throw (RuntimeException);
        /// Setzt die Geschwindigkeit der Soundquelle.
        void setVelocity(const Ogre::Vector3&) throw (RuntimeException);
        /// Laedt die Soundquelle.
        virtual void load() = 0;
        /// Entlaedt die Soundquelle.
        virtual void unload();
        
        /// Spielt den Sound ab.
        virtual void play(unsigned int msec = 0) throw (RuntimeException);
        /// Pausiert den Sound.
        virtual void pause() throw (RuntimeException);
        /// Stoppt den Sound.
        virtual void stop(unsigned int msec = 0) throw (RuntimeException);
        /// Zurueck auf Anfang.
        virtual void rewind() throw (RuntimeException);
        /// Den Zustand des Sounds zurueckgeben.
        const ALenum getState() const throw (RuntimeException);
        
};

}
#endif
