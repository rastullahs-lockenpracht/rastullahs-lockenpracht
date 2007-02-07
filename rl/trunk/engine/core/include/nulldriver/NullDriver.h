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
#ifndef NULLDRIVER_H_
#define NULLDRIVER_H_

#include "CorePrerequisites.h"
#include "SoundDriver.h"

namespace rl
{

/** Diese Klasse ist der Nulltreiber, der immer
 * vorhanden ist und einfach nichts ausgibt.
 */
class _RlCoreExport NullDriver : public rl::SoundDriver
{
public:
    /// Der Treibername
    static Ogre::String NAME;
    /// Der Konstruktor
    NullDriver(Ogre::ResourceManager* soundResourceManager);
    /// Der Destruktor
    ~NullDriver();
    /// Ist der Treiber angeschaltet?
    bool isDriverAvailable();
    /// Initialisiere den Treiber.
    virtual bool initialize();
    /// Deinitialisiere den Treiber.
    virtual void shutdown();
    /// Der Name des Treibers
    virtual Ogre::String getName() const;
    /// Update-Aufgaben erledigen
    virtual void update();
    /// Einen Sound-Stream mit Resource erzeugen
    virtual Sound *createStream(const SoundResourcePtr &res);
    /// Einen Sound-Sample mit Resource erzeugen
    virtual Sound *createSample(const SoundResourcePtr &res);
    /// Einen Soundlistener erzeugen
    virtual ListenerMovable *createListener(const Ogre::String &name);
    /// Setzt den Faktor f, mit der die Lautst�rke nach der Formel 1/(f*Entfernung) abnimmt
    virtual void setRolloffFactor(const Ogre::Real&);
    virtual const Ogre::Real getRolloffFactor();
};

}

#endif /*NULLDRIVER_H_*/
