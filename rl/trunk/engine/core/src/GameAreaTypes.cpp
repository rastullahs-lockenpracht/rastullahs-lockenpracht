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

#include "GameAreaTypes.h"

#include "World.h"
#include "CoreSubsystem.h"

using namespace Ogre;

namespace rl {

    GameSphereAreaType::GameSphereAreaType(Vector3 center, Real radius, unsigned long mask)
    {
        m_SphereQuery = CoreSubsystem::getSingleton().getWorld()->
            getSceneManager()->createSphereQuery( Sphere(center,radius), mask );

        // Keine Welt-Geometrie erwünscht, nur Movables
        m_SphereQuery->setWorldFragmentType( SceneQuery::WFT_NONE );
    }

    GameSphereAreaType::~GameSphereAreaType()
    {
        m_SphereQuery->clearResults();
        CoreSubsystem::getSingleton().getWorld()->getSceneManager()->destroyQuery( m_SphereQuery );
    }

    ActorList GameSphereAreaType::performQuery(  )
    {
        SceneQueryResult rs = m_SphereQuery->execute();
        SceneQueryResultMovableList movList = rs.movables;
        
        ActorList retList;
        
        // Durch die Ergebnis Liste iterieren
        SceneQueryResultMovableList::iterator it;
        for (it = movList.begin(); it != movList.end(); ++it) 
        {
            MovableObject* mov = *it;  
            if( mov->getUserObject() != NULL )
            {
                // Zur Zeit sind die einzigen an Movables geknüpfte Objekte Actoren
                Actor* act = dynamic_cast<Actor*>( mov->getUserObject() );
                retList.insert( retList.end(), act );
            }
        }

        return retList;
    }

}

