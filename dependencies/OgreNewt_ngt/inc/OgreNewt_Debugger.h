/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

		by Walaber

*/
#ifndef _INCLUDE_OGRENEWT_DEBUGGER
#define _INCLUDE_OGRENEWT_DEBUGGER


#include <Ogre.h>
#include <Newton.h>
#include "OgreNewt_World.h"

namespace OgreNewt
{

	//! For viewing the Newton rigid bodies visually.
	/*!
		This class implements a debug view of the Newton world.  it is a Singleton!
	*/
	class _OgreNewtExport Debugger
	{
	
	public:
		~Debugger();

		//! get the singleton reference
		static Debugger& getSingleton();

		//! init the debugger.
		/*
			\param smgr pointer to your Ogre::SceneManager
		*/
		void init( Ogre::SceneManager* smgr );

		//! de-init the debugger (cleantup)
		void deInit();

		//! show the newton world
		/*!
			Draws the Newton world as 3D lines.
			\param world pointer to the OgreNewt::World
		*/
		void showLines( OgreNewt::World* world );

		//! remove lines drawn.
		void hideLines();
	
	protected:
		Debugger();
	
	private:
		Ogre::SceneNode*		m_debugnode;
		Ogre::ManualObject*		m_debuglines;
	
		

		static void _CDECL newtonPerBody( const NewtonBody* body );
		static void _CDECL newtonPerPoly( void* userData, int vertexCount, const float* faceVertec, int id );
	};

}	// end namespace OgreNewt


#endif	// _INCLUDE_OGRENEWT_DEBUGGER

