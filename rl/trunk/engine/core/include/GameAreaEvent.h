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
*/
class _RlCoreExport GameAreaEvent : public virtual EventObject 
{
public:
    static const unsigned int AREA_ENTERED = 450;
    static const unsigned int AREA_LEFT = 451;

    GameAreaEvent( GameAreaEventSource* src, const unsigned int reason );
    virtual ~GameAreaEvent() {};

    void setProvokingActor(Actor* act ) {  m_Actor = act; };
    Actor* getProvokingActor() const { return m_Actor; };
    GameAreaEventSource* getGameAreaEventSource() const;
private:
    Actor* m_Actor;
};
}

#endif
