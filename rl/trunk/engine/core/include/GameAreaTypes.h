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

#include <map>

#include "Actor.h"
#include "CorePrerequisites.h"

namespace rl {

typedef std::map<const Ogre::String,Actor*> ActorMap;
typedef std::pair<const Ogre::String,Actor*> ActorPair;
class GameAreaEventSource;

/** 
*/
class _RlCoreExport GameAreaType
{
public:
    virtual ActorMap performQuery(  ) = 0;

    virtual unsigned long getQueryMask() const = 0;
    virtual void setQueryMask( unsigned long mask ) = 0;

    void addQueryFlag( unsigned long flag  );
    void removeQueryFlag( unsigned long flag );

    virtual void setQueryPosition( const Ogre::Vector3& vec ) = 0;
    virtual const Ogre::Vector3& getQueryPosition() const = 0;

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

    virtual ActorMap performQuery(  );

    virtual unsigned long getQueryMask() const;
    virtual void setQueryMask( unsigned long mask );

    virtual void setQueryPosition( const Ogre::Vector3& vec );
    virtual const Ogre::Vector3& getQueryPosition() const;    
private:
    Ogre::SphereSceneQuery* m_SphereQuery;
};

}

#endif
