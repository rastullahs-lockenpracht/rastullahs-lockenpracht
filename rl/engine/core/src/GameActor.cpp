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
	String args[] = {name};
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
