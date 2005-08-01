/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __GameAreaEvent_H__
#define __GameAreaEvent_H__

#include "Actor.h"
#include "EventObject.h"

#include "CorePrerequisites.h"

namespace rl {

class GameAreaEventSource;

/** 
 *  GameAreaEvent
 *  Ein Event der ausgelöst wird, wenn ein Actor ein zuvor beim GameEventManager
 *  registriertes Areal betritt/verlässt.
 *  Dabei wird für jeden Actor der in der Abfragezeit den Radius verlassen/betreten
 *  hat ein einzelner Event ausgelöst. Dabei werden zuerst die verlassenden und danach
 *  die betretenden Actoren übermittelt.
 *
 *  @see GameAreaListener, GameAreaEventSource, GameEventManager, GameAreaTypes
 */
class _RlCoreExport GameAreaEvent : public virtual EventObject 
{
public:
    /// Das Areal wurde betreten
    static const unsigned int AREA_ENTERED = 450;
    /// Das Areal wurde verlassen
    static const unsigned int AREA_LEFT = 451;

    /** Der Standardkonstruktor
      * @param src Die Eventquelle
      * @param reason Der Grund des Events
      */
    GameAreaEvent( GameAreaEventSource* src, const unsigned int reason );
    /// Standarddestruktor
    virtual ~GameAreaEvent() {};

    /// Setzt den verursachenden Actor des Events
    void setProvokingActor(Actor* act ) {  mActor = act; };
    /// Gibt den verursachenden Actor des Events zurück
    Actor* getProvokingActor() const { return mActor; };
    /// Gibt die Eventquelle zurück
    GameAreaEventSource* getSource() const;
private:
    /// Der verursachende Actor
    Actor* mActor;
};
}

#endif
