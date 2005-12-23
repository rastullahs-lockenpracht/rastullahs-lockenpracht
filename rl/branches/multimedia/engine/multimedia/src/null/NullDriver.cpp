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
#include "NullDriver.h"

namespace rl
{

/** 
 * Konstruktor
 * @author JoSch
 * @date 12-23-2005
 */
NullDriver::NullDriver()
{
}

/** 
 * Destruktor
 * @author JoSch
 * @date 12-23-2005
 */
NullDriver::~NullDriver()
{
}

/** 
 * Zeigt an, ob der Treiber funktionsfähig ist.
 * @return Immer TRUE.
 * @author JoSch
 * @date 12-23-2005
 */
bool NullDriver::isDriverAvailable()
{
    return true;
}

/** 
 * Starte den Nulltreiber und mache alle Initialisierung.
 * @author JoSch
 * @date 12-23-2005
 */
void NullDriver::init()
{
}

/** 
 * Beeende den Nulltreiber und gebe alle Resourcen frei.
 * @author JoSch
 * @date 12-23-2005
 */
void NullDriver::deInit()
{
}

/** 
 * Gibt den Anzeigename des Treiber zurück.
 * @return Anzeigename
 * @author JoSch
 * @date 12-23-2005
 */
CeGuiString NullDriver::getName() const
{
    return "Nulltreiber";
}


}
