/* EventObject.h - Das Ereignis, das verschickt wird.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License as published by
 *  the .
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not
 */
#ifndef EVENTOBJECT_H
#define EVENTOBJECT_H

#include "EventSource.h"

namespace rl {
/**
 * Dieses Klasse ist eine Basisklasse fuer alle Ereignisse.
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
class EventObject {
private:
   /// Die Quelle des Ereignisses.
   EventSource *mSource; 
    
public:
    /// Der Konstruktor.
	EventObject(EventSource *source);
    /// Der Destruktor.
	virtual ~EventObject() ;
};

}
#endif // EVENTOBJECT_H
