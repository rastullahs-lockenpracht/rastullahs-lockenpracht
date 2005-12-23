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
#ifndef SOUNDDRIVER_H_
#define SOUNDDRIVER_H_

#include "MultimediaPrerequisites.h"

namespace rl
{

/**
 * Diese Klasse dient zur Definition einer Schnittstelle
 * zu den möglichen Soundbibliotheken. Sie geben Preis, ob
 * ein Treiber vorhanden ist oder nicht. Der Nulltreiber
 * existiert dabei immer.
 */
class _RlMultimediaExport SoundDriver
{
public:
    /// Der Konstruktor
	SoundDriver();
    /// Der Destruktor
	virtual ~SoundDriver();
    /// Zeigt an, ob der Treiber funktionsfähig ist.
    virtual bool isDriverAvailable() = 0;
    /// Initialisiere den Treiber
    virtual void init() = 0;
    /// Beende den Treiber
    virtual void deInit() = 0;
    /// Der Name des Treibers
    virtual CeGuiString getName() const = 0;
};

}

#endif /*SOUNDDRIVER_H_*/
