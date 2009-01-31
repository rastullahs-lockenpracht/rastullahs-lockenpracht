/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

		by Walaber

*/

#ifndef _INCLUDE_OGRENEWT_BODYINAABBITERATOR
#define _INCLUDE_OGRENEWT_BODYINAABBITERATOR


#include "OgreNewt_Prerequisites.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>

// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{

class World;
class Body;


//! Iterate through all bodies in a specific AABB in the world.
/*!
	this class is an easy way to loop through all bodies in an AABB in the world, performing some kind of action.
    if you want to iterate through all bodies, use the world->getFirstBody and body->getNext functions
    warning: this class is not thread safe
*/
class _OgreNewtExport BodyInAABBIterator
{
public:
	//! function to be called for all bodies
	/*!
		This function will be called for every body iterated.  you can put any functionality you might want inside this function.
	*/
	typedef boost::function<void(Body*)> IteratorCallback;

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
	void go( const Ogre::AxisAlignedBox &aabb, IteratorCallback callback )
	{
		m_callback = callback;
        NewtonWorldForEachBodyInAABBDo(m_world->getNewtonWorld(), &aabb.getMinimum().x, &aabb.getMaximum().x, newtonIterator);
        
	}
	template <class c> void go( const Ogre::AxisAlignedBox &aabb, boost::function<void(c*, Body*)> callback, c* instancedClassPointer )
	{
		go ( aabb, boost::bind(callback, instancedClassPointer, _1) );
	}

	//! get the singleton.
	/*!
		The body iterator is a singleton class, only one instance should exist per application.  you can perform many different
		kinds of iterations by using different callback functions.
	*/
	static BodyInAABBIterator& getSingleton()
	{
		static BodyInAABBIterator instance;
		return instance;
	}

	//! destructor
	~BodyInAABBIterator() {}

protected:

	BodyInAABBIterator()
	{
		m_world = NULL;
		m_callback = NULL;
	}

	static void _CDECL newtonIterator( const NewtonBody* body )
	{
		OgreNewt::Body* bod = (OgreNewt::Body*)NewtonBodyGetUserData( body );
		getSingleton().m_callback( bod );
	}

	const OgreNewt::World*			m_world;
	IteratorCallback				m_callback;
    
};


}	// end NAMESPACE OgreNewt

#endif	// _INCLUDE_OGRENEWT_BODYINAABBITERATOR

