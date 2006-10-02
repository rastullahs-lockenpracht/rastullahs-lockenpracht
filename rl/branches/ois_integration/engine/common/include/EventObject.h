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

#ifndef EVENTOBJECT_H
#define EVENTOBJECT_H

#include "CommonPrerequisites.h"

#include "EventSource.h"

namespace rl {
/**
 * Dieses Klasse ist eine Basisklasse fuer alle Ereignisse.
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
class _RlCommonExport EventObject {
private:
   /// Die Quelle des Ereignisses.
   EventSource *mSource; 
   /// Die ID des Grundes für den Event.
   unsigned int mReason;
   
    
public:
    /// Der Konstruktor.
	EventObject(EventSource *source);
	/// Der Konstruktor.
	EventObject(EventSource *source, const unsigned int reason );
    /// Der Destruktor.
	virtual ~EventObject();
   
   /// Die Ereignisquelle zurueckgeben
   EventSource *getSource() const;
   /// Die ID des Grundes für den Event.
   const unsigned int getReason() const;
   /// Die ID setzen
   void setReason(const unsigned int reason);
};

}
#endif // EVENTOBJECT_H
