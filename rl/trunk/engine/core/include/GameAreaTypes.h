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

#ifndef __GameAreaTypes_H__
#define __GameAreaTypes_H__

#include <list>

#include "Actor.h"
#include "CorePrerequisites.h"

namespace rl {

typedef std::list<Actor*> ActorList;
class GameAreaEventSource;

/** 
*/
class _RlCoreExport GameAreaType
{
public:
    virtual ActorList performQuery(  ) = 0;

    void setGameAreaEventSource( GameAreaEventSource * gaes )
        { m_GameAreaEventSource = gaes; };
    GameAreaEventSource* getGameAreaEventSource( ) const 
        { return m_GameAreaEventSource; };
private:
    GameAreaEventSource* m_GameAreaEventSource;
};

class _RlCoreExport GameSphereAreaType : public GameAreaType
{
public:
    GameSphereAreaType(Ogre::Vector3 center, Ogre::Real radius, unsigned long mask = 0xFFFFFFFF );
    virtual ~GameSphereAreaType();

    virtual ActorList performQuery(  );
private:
    Ogre::SphereSceneQuery* m_SphereQuery;
};

}

#endif
