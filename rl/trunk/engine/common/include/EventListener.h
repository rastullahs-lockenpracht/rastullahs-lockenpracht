/* EventListener.h - Ein Objekt, das auf Ereignisse wartet.
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
#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

namespace rl {
/**
 * Diese Klasse ist die Basisklasse fuer Objekte, die an Ereignisse interessiert
 * sind.
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event>
class EventListener{
public:
    /// Der Konstruktor.
	EventListener();
    /// Der Destruktor.
	virtual ~EventListener();
    /// Hier wird das Objekt benachrichtigt.
    virtual bool eventRaised(Event *anEvent);
};

}
#endif // EVENTLISTENER_H
