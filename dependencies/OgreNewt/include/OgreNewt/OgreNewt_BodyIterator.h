/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

		by Walaber

*/

#ifndef _INCLUDE_OGRENEWT_BODYITERATOR
#define _INCLUDE_OGRENEWT_BODYITERATOR

#include <Newton.h>

#include "OgreNewt_World.h"
#include "OgreNewt_Body.h"


// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{


//! Iterate through all bodies in the world.
/*!
	this class is an easy way to loop through all bodies in the world, performing some kind of action.
*/
class _OgreNewtExport BodyIterator
{
public:
	//! function to be called for all bodies
	/*!
		This function will be called for every body in the world.  you can put any functionality you might want inside this function.
	*/
	typedef void(*IteratorCallback)( Body* me );

	//! init the singleton class.
	/*!
		This function must be called before any iterator functions are called!
		\param world pointer to the OgreNewt::World.  
	*/
	void Init( const OgreNewt::World* world ) { m_world = world; }

	//! perform an iteration
	/*!
		will call the provided function for all bodies in the world.
		\param callback pointer to a function to be used
	*/
	void go( IteratorCallback callback )
	{
		m_callback = callback;

		NewtonWorldForEachBodyDo( m_world->getNewtonWorld(), newtonIterator );
	}

	//! get the singleton.
	/*!
		The body iterator is a singleton class, only one instance should exist per application.  you can perform many different
		kinds of iterations by using different callback functions.
	*/
	static BodyIterator& getSingleton()
	{
		static BodyIterator instance;
		return instance;
	}

	//! destructor
	~BodyIterator() {}

protected:

	BodyIterator()
	{
		m_world = NULL;
		m_callback = NULL;
	}

	static void _CDECL newtonIterator( const NewtonBody* body )
	{
		OgreNewt::Body* bod = (OgreNewt::Body*)NewtonBodyGetUserData( body );

		(*getSingleton().m_callback)( bod );
	}

	const OgreNewt::World*			m_world;
	IteratorCallback				m_callback;
    
};


}	// end NAMESPACE OgreNewt

#endif	// _INCLUDE_OGRENEWT_BODYITERATOR

