/* EventCaster.h - Ein Objekt, das Ereignisse verschickt.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#ifndef EVENTCASTER_H
#define EVENTCASTER_H

#include <set>
#include "EventListener.h"

using namespace std;

namespace rl {
/**
 * Diese Klasse ist die Basisklasse fuer Objekte, die Ereignisse verschicken.
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <class Event>
class EventCaster {
private:
    set<EventListener<Event>*> mListeners; 
public:
    /// Der Konstruktor
	EventCaster();
    /// Der Destruktor
	virtual ~EventCaster();
   /// Fuege einen EventListener hinzu.
   void addEventListener(EventListener<Event>* newListener);
   /// Loesche einen EventListener.
   void removeEventListener(EventListener<Event>* aListener);
   /// Ein Ereignis verteilen.
   void dispatchEvent(Event *anEvent);
};
     
}
#endif // EVENTCASTER_H
