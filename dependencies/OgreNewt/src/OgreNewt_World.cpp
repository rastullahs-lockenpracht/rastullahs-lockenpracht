#include "OgreNewt_World.h"
#include "OgreNewt_MaterialID.h"
#include "OgreNewt_BodyInAABBIterator.h"

namespace OgreNewt
{


// Constructor
World::World() :
    m_bodyInAABBIterator(this),
    m_debugger(this)
{
	m_limits = Ogre::AxisAlignedBox(Ogre::Vector3(-100,-100,-100), Ogre::Vector3(100,100,100));

	m_world = NewtonCreate( NULL, NULL );

	if (!m_world)
	{
		// world not created!
	}

	NewtonWorldSetUserData( m_world, this );

	// create the default ID.
	m_defaultMatID = new OgreNewt::MaterialID( this, NewtonMaterialGetDefaultGroupID( m_world ) );

	m_leaveCallback = NULL;
}

// Destructor
World::~World()
{
	if (m_defaultMatID)
		delete m_defaultMatID;

	if (m_world)
		NewtonDestroy( m_world );
}

// update
void World::update( Ogre::Real t_step )
{
	NewtonUpdate( m_world, (float)t_step );
}

void World::setWorldSize( const Ogre::Vector3& min, const Ogre::Vector3& max )
{
	NewtonSetWorldSize( m_world, (float*)&min.x, (float*)&max.x );
	m_limits = Ogre::AxisAlignedBox(min, max);
}

void World::setWorldSize( const Ogre::AxisAlignedBox& box )
{
	NewtonSetWorldSize( m_world, (float*)&box.getMinimum(), (float*)&box.getMaximum() );
	m_limits = box;
}

void World::setLeaveWorldCallback( LeaveWorldCallback callback )
{
	if (!m_leaveCallback)
	{
		NewtonSetBodyLeaveWorldEvent( m_world, World::newtonLeaveWorld );
	}

	if (callback)
	{
		m_leaveCallback = callback;
	}
	else
	{
		NewtonSetBodyLeaveWorldEvent( m_world, NULL );
		m_leaveCallback = NULL;
	}
}

void _CDECL World::newtonLeaveWorld( const NewtonBody* body, int threadIndex )
{
	OgreNewt::World* me = (OgreNewt::World*)NewtonWorldGetUserData( NewtonBodyGetWorld( body ) );

	if (me->m_leaveCallback)
	{
		OgreNewt::Body* b = (OgreNewt::Body*)NewtonBodyGetUserData( body );
		me->m_leaveCallback( b, threadIndex );
	}
}

Body* World::getFirstBody() const
{
    NewtonBody* body = NewtonWorldGetFirstBody( m_world );
    if( body )
        return (Body*) NewtonBodyGetUserData(body);

    return NULL;
}


// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------


}

