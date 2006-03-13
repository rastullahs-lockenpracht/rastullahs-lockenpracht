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
#ifdef WITH_OAL
#ifndef OALDRIVER_H_
#define OALDRIVER_H_

#include "MultimediaPrerequisites.h"
#include "SoundDriver.h"

namespace rl
{

/** Diese Klasse ist der Treiber, der OpenAL zur
 * Ausgabe benutzt.
 */
class _RlMultimediaExport OalDriver : public rl::SoundDriver
{
public:
    /// Der Konstruktor
	OalDriver();
    /// Der Destruktor
	virtual ~OalDriver();
    /// Ist der Treiber angeschaltet?
    virtual bool isDriverAvailable();
    /// Initialisiere den Treiber.
    virtual void init();
    /// Deinitialisiere den Treiber.
    virtual void deInit();
    /// Der Name des Treibers
    virtual CeGuiString getName() const;
    /// Update-Aufgaben erledigen
    virtual void update();
    /// Einen Sound-Stream mit Name erzeugen
    virtual Sound *createStream(const Ogre::String &name);
    /// Einen Sound-Stream mit Resource erzeugen
    virtual Sound *createStream(const SoundResourcePtr &res);
    /// Einen Sound-Sample mit Name erzeugen
    virtual Sound *createSample(const Ogre::String &name);
    /// Einen Sound-Sample mit Resource erzeugen
    virtual Sound *createSample(const SoundResourcePtr &res);
    /// Einen SoundChannel erzeugen
    virtual SoundChannel *createChannel(Sound *channel, const Ogre::String &name);
    
protected:
    /// Informationen über den Treiber ausgeben
    virtual void printData();
};

}

#endif /*OALDRIVER_H_*/
#endif // WITH_OAL
