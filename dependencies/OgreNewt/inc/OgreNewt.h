/*
	OgreNewt library

	connects Ogre with the Newton Game Dynamics physics library


		by Walaber (http://walaber.com)

	main header file.

	current version:  2.00  ( I only raised the version number to indicate that this is for Newton2.0! )

		Newton version: 2.0 beta (31.01.2009)

		Ogre version: Shoggoth (1.6.x)

*/

#ifndef _INCLUDE_OGRENEWT_
#define _INCLUDE_OGRENEWT_


#include "OgreNewt_World.h"
#include "OgreNewt_Collision.h"
#include "OgreNewt_Body.h"
#include "OgreNewt_CollisionPrimitives.h"
#include "OgreNewt_CollisionSerializer.h"
#include "OgreNewt_MaterialID.h"
#include "OgreNewt_MaterialPair.h"
#include "OgreNewt_ContactCallback.h"
#include "OgreNewt_ContactJoint.h"
#include "OgreNewt_Vehicle.h"
#include "OgreNewt_PlayerController.h"
#include "OgreNewt_RayCast.h"

#include "OgreNewt_Joint.h"
#include "OgreNewt_BasicJoints.h"

#include "OgreNewt_Tools.h"
#include "OgreNewt_BodyInAABBIterator.h"
#include "OgreNewt_Debugger.h"
#include "OgreNewt_BasicFrameListener.h"

/*! \mainpage OgreNewt Library version 2.00

	\section into_sec Introduction

	OgreNewt is an OOP wrapper for integrating the Newton Dynamics Physics SDK with OGRE.

    \section problems Problems and missing features in this version

    Problems and missing features in this version
        - PlayerController not finished
        - the convexcast seems to have difficulties with more than one contact (I've segfault when debugging it with the OgreNewt::Debugger...)
          I added a hack, it only creates one contact at the maximum at the moment!
        - this library supports the newton functions for multithreading but there are several functions/classes in this
          library itself, that are not thread safe (I don't need multiple threads)
          especially the world-updates are not threadsafe in ogre (see newton forum, someone posted a solution there!)
        - the copied MovableText class (in Tools::OgreAddons) seems to have some graphics error (it's used
          to show the debugging information above each body)
        - I didn't test several features:
            - vehicle
            - joints
            - collsions-serializer
            - treecollision raycastcallback 
        - shared pointers for collisions (atm CollisionPtr := Collision*, but should be Ogre::SharedPtr<Collision>, but there was some problem...)


	\section new New in this version

    New in this version
        - this version uses cmake > 2.6.2!
        - several interface-breaking changes:
            - the ContactCallback passes a ContactJoint as argument that can be used to iterate through all contacts
              (the callback is not called for each contact any more, but for each pair of colliding bodies!)
            - added a lots of new simple "iterators":
                - for Bodies use body = World->getFirstBody() and body = body->getNext()
                - for the 
            - BodyIterator renamed to BodyInAABBIterator, it does now only iterate through bodies in a specific AABB
            - removed "Singleton-classes", the debugger and the BodyInAABBIterator are now part of a World
              (call world->getDebugger() / world->getBodyInAABBIterator() to use these classes!)
        - added support for trigger-volumes (ConvexCollision::setAsTriggerVolume ...)
        - added support for convexcasts
        - added additional debugging features:
            - raycast/convexcast debugging
            - information about each body as billboard above it
        - a lot of minor changes and functions added and renamed


	previous changes...
		- updated to Newton version 2.0!!  Many big improvements and changes to the library, and OgreNewt now supports it.
		- several interface-breaking changes:
			- updated to support multithreading support now native to Newton.
			- MomentOfInertia namespace and helper functions removed entirely.  Newton can now calculate inertia and center of
			  mass automatically, use ConvexCollision::calculateInertialMatrix, and multiply by objects Mass.
			- Body::attachToNode has been renamed to attachNode, better implying the relationship between the body and the Ogre::Node.
			- BodyIterator class has been upgraded to use boost::function, allowing the same callback flexibility as other callbacks.

		- bug fix - fixed problem with TreeCollisionSceneParser that wasn't adjusting to different scales properly.
		- optimization - fixed several classes to only write debug data to the LOG when compiled in DEBUG mode:
			- Custom2DJonit
			- BasicFrameListener
		- added the concept of "face-winding" to all TreeCollision classes.  should not cause any breaking changes to code, but allows more flexibility to change the "direction" of a face in treecollisions.
		- OgreNewt and it's demos are now fully compatible with Ogre 1.4.x (Eihort).
		- Addition of script files for compiling under linux (Scons)
		- slight modification to the ContactCallback class to allow access to Newton structures from the outside.
		- some cosmetic changes to make the wrapper compile more easily under Linux and other non-windows platforms.
		- updated to work with the release version of Ogre dagon (1.2)
		- updated to work with version 1.53 of Newton
		- raycast now has an optional pre-filter to ignore specific bodies from the raycast.
		- various other clean-up!
		- moved callback system to much more OOP-friendly "boost::function" system.  callbacks can now be instance-specific member functions!
		- several other small fixes to project files, etc.
		- automatic convex hull generation system added to supplementary ragdoll class (see demo08).
		- documentation updated.
		- updated to Newton version 1.5
		- added this documentation!
		- added buoyancy functionality.
		- New Buoyancy demo.
		- New Ragdoll demo, using a seperate class to implement ragdoll functionality.
		- added basic classes for CustomJoints, and a simple demo showing how they work.
		- added several prebuilt custom joints, including pulleys and gears, and a general 2D joint.
		- added calculateInertialMatrix to ConvexCollision class.
		- added set/getCenterOfMass to Body class
		- changed angle-related functions in BasicJoint classes to use Ogre::Radian instead of Ogre::Real.
		- added ConvexModifierCollision class, which allows for arbitrarily scaled collision primitives.
		- added several low-level collision functions, see the CollisionTools namespace.
		- fixed errors causing run-time crash on Slider and Universal joints.
		- added LeaveWorldCallback functionality to World class.

	\section install Installation

	OgreNewt comes with a project for MSVC++ 7.1 and MSVC++ 8.0 (2005).  unfortunately I have no other IDE's or compilers setup on my system, so I
	cannot provide any other projects.  the MSVC solution should build out of the box, assuming you take a few things into
	consideration.

	unzip this in the ogreaddons directory alongside your "ogrenew" directory.  to compile correctly, it also assumes you have the Newton SDK installed in another
	directory alongside this one called "NewtonSDK".

	for example something like this:

	- c:/programming/Ogre/ogrenew						<- ogre installation
	- c:/programming/Ogre/ogreaddons/OgreNewt			<- OgreNewt main directory.
	- c:/programming/NewtonSDK							<- Newton SDK.
	- c:/programming/tinyxml							<- tinyxml library (needed for ragdoll demo only)

	\section compiling Compiling

	OgreNewt compiles by default to a static library for linking to your project.  However you can also compile OgreNewt into a dynamic DLL on the 
	Windows platform by using the Debug_DLL and Release_DLL solution build configurations in the MSVC solutions.

	\section license License

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.


	well, that's about it!
	have fun!

	-walaber

*/

#endif

