/* Copyright 2004 Jochen Sch?fer */
#ifndef SNDRESOURCE_H
#define SNDRESOURCE_H

#include "SoundPrerequisites.h"
#include "Ogre.h"

namespace rl {
 
 
/** Diese Basisklasse kapselt eine OpenAl++-Source fuer
 * den ResourceManager von Ogre
 * @author Josch
 * @date 06-19-2004
 * @date 07-23-2004
 * @version 2.0
 */
class _RlSoundExport SndResource: public Resource {
    protected:
        /// Die gekapselte Soundquelle
        ALuint mSource;
        /// Die Buffer, die wir benutzen
        ALuint *mBuffers;
        /// Wieviele Buffer werden benutzt.
        short mBufferCount;

        /// Ueperpruefen, ob Fehler aufgetreten ist.
        void check() const throw (RuntimeException);

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
        virtual void play() throw (RuntimeException);
        /// Pausiert den Sound.
        virtual void pause() throw (RuntimeException);
        /// Stoppt den Sound.
        virtual void stop() throw (RuntimeException);
        /// Zurueck auf Anfang.
        virtual void rewind() throw (RuntimeException);
        /// Den Zustand des Sounds zurueckgeben.
        const ALenum getState() const throw (RuntimeException);
        
};

}
#endif
