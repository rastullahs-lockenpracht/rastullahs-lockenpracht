#include <OgreNewt_MaterialPair.h>
#include <OgreNewt_World.h>
#include <OgreNewt_MaterialID.h>

namespace OgreNewt
{

	
MaterialPair::MaterialPair( const World* world, const MaterialID* mat1, const MaterialID* mat2 )
{
	m_world = world;
	id0 = mat1;
	id1 = mat2;
}

MaterialPair::~MaterialPair()
{
}


void MaterialPair::setContactCallback( OgreNewt::ContactCallback* callback )
{
	// set the material callbacks to the functions inside the ContactCallback class.
	NewtonMaterialSetCollisionCallback( m_world->getNewtonWorld(), id0->getID(), id1->getID(), callback,
		callback->contactBegin,
		callback->contactProcess,
		callback->contactEnd );
}



}
