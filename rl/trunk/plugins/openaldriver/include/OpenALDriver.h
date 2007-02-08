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
#ifndef _OPENALDRIVER_H_
#define _OPENALDRIVER_H_

#include "OpenALDriverPrerequisites.h"
#include "SoundDriver.h"

namespace rl
{

    /** Diese Klasse ist der Treiber, der OpenAL zur
     * Ausgabe benutzt.
     */
    class _RlOpenALDriverExport OpenALDriver : public rl::SoundDriver
    {
    public:
        /// Der Treibername
        static Ogre::String NAME;
        /// Der Konstruktor
        OpenALDriver(Ogre::ResourceManager* soundResourceManager);
        /// Der Destruktor
        virtual ~OpenALDriver();
        /// Ist der Treiber angeschaltet?
        virtual bool isDriverAvailable();
        /// Initialisiere den Treiber.
        virtual bool initialize();
        /// Deinitialisiere den Treiber.
        virtual void shutdown();
        /// Der Name des Treibers
        virtual Ogre::String getName() const;
        /// Update-Aufgaben erledigen
        virtual void update();
        /// Einen Soundlistener erzeugen
        virtual ListenerMovable *createListener(const Ogre::String &name);

        /// Setzt den Faktor f, mit der die Lautstärke nach der Formel 1/(f*Entfernung) abnimmt
        virtual void setRolloffFactor(const Ogre::Real& f);
        virtual const Ogre::Real getRolloffFactor();

    protected:
        virtual Sound* createStreamImpl(const SoundResourcePtr &res);
        virtual Sound* createSampleImpl(const SoundResourcePtr &res);

    private:
        /// Informationen über den Treiber ausgeben
        virtual void printData() const;
    };
}

#endif /*OALDRIVER_H_*/
