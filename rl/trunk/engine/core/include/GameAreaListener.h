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

#ifndef __GameAreaListener_H__
#define __GameAreaListener_H__

#include "EventListener.h"
#include "GameAreaEvent.h"

#include "CorePrerequisites.h"

namespace rl {

/** 
*/
class _RlCoreExport GameAreaListener : public virtual EventListener<GameAreaEvent>
{
public:
    virtual ~GameAreaListener() {};

    virtual void areaLeft(GameAreaEvent *anEvent) const = 0;
    virtual void areaEntered(GameAreaEvent *anEvent) const = 0;

    virtual bool eventRaised(GameAreaEvent *anEvent) const;
};
}

#endif
