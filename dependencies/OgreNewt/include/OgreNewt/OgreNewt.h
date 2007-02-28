/*
	OgreNewt library

	connects Ogre with the Newton Game Dynamics physics library


		by Walaber (http://walaber.com)

	main header file.

	current version:  0.10

		Newton version: 1.53

		Ogre version: Eihort (1.4.x)

*/

#ifndef _INCLUDE_OGRENEWT_
#define _INCLUDE_OGRENEWT_

#include "OgreNewt_Prerequisites.h"

#include "OgreNewt_World.h"
#include "OgreNewt_Collision.h"
#include "OgreNewt_Body.h"
#include "OgreNewt_CollisionPrimitives.h"
#include "OgreNewt_TreeCollisionSerializer.h"
#include "OgreNewt_MaterialID.h"
#include "OgreNewt_MaterialPair.h"
#include "OgreNewt_ContactCallback.h"
#include "OgreNewt_Vehicle.h"
#include "OgreNewt_RayCast.h"

#include "OgreNewt_Joint.h"
#include "OgreNewt_BasicJoints.h"

#include "OgreNewt_Tools.h"
#include "OgreNewt_BodyIterator.h"
#include "OgreNewt_Debugger.h"


/*! \mainpage OgreNewt Library version 0.10

	\section into_sec Introduction

	OgreNewt is an OOP wrapper for integrating the Newton Dynamics Physics SDK with OGRE.

	\section new New in this Version

	New in this version!
		- OgreNewt and it's demos are now fully compatible with Ogre 1.4.x (Eihort).
		- moved callback system to much more OOP-friendly "fastdelegate" system.  callbacks can now be instance-specific member functions!
		- Addition of script files for compiling under linux (Scons)
		- slight modification to the ContactCallback class to allow access to Newton structures from the outside.

	previous changes...
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

	note that if you compile OgreNewt as a dynamic DLL and the default debugger, you will need to manually link to the Line3D.obj file by
	either adding Line3D.cpp to the project, or compiling it separately.

	\section license License

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.


	well, that's about it!
	have fun!

	-walaber

*/

#endif

