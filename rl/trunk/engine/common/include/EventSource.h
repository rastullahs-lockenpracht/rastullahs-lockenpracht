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

#ifndef EVENTSOURCE_H
#define EVENTSOURCE_H

#include "CommonPrerequisites.h"

namespace rl {
/**
 * Diese Klasse dient als Basisklasse fuer Ereignisquellen
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
class _RlCommonExport EventSource {
public:
    /// Der Konstruktor.
   EventSource();
    /// Der Destruktor.
   virtual ~EventSource();
};

} 
#endif // EVENTSOURCE_H
