/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "GameActor.h"

namespace rl {

const String GameActor::TYPENAME="GameActor";

GameActor::GameActor( const String& name, Entity* entity) 
	:	MeshActor(name,entity) , 
		AudibleActor(name),
		Actor(name)
{
}

GameActor::GameActor( const String& name, SceneNode* parentSceneNode, Entity* entity)
	:	MeshActor(name,parentSceneNode,entity) , 
		AudibleActor(name,parentSceneNode),
		Actor(name,parentSceneNode)
{
//	String args[] = {name};
}

void GameActor::playSound(const String &name)
{
	AudibleActor::playSound(name);
}

GameActor::~GameActor()
{
}

const String& GameActor::getTypeName()
{
	return TYPENAME;
}

/**
 * @param x, y, z Die neue Position des Aktors.
 * @author JoSch
 * @date 05-04-2004
 */
void GameActor::setPosition(Real x, Real y, Real z)
{
    AudibleActor::setPosition(x, y, z);
}

/**
 * @param orientation Die neue Orientierung.
 * @author JoSch
 * @date 05-04-2004
 */
void GameActor::setOrientation(const Quaternion& orientation)
{
    AudibleActor::setOrientation(orientation);
}

bool GameActor::isSimpleOgreActor()
{
    return ( AudibleActor::isSimpleOgreActor() );
}

}
