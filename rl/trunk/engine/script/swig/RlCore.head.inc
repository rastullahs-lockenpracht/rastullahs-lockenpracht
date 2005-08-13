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

%{
//----------- Includes - Core -----------
#include "FixRubyHeaders.h"
#include "CorePrerequisites.h"
#include "ActorManager.h"
#include "Actor.h"
#include "MeshObject.h"
#include "SoundObject.h"
#include "Sound.h"
#include "ListenerObject.h"
#include "ListenerMovable.h"
#include "CameraObject.h"
#include "LightObject.h"
#include "ParticleSystemObject.h"
#include "CoreSubsystem.h"
#include "PhysicalThing.h"
#include "PhysicsManager.h"
#include "GameEventManager.h"
#include "GameAreaEventSource.h"
#include "GameAreaListener.h"
#include "GameAreaEvent.h"
#include "GameAreaTypes.h"
#include "AnimationManager.h"
#include "AnimationListener.h"
#include "Animation.h"
#include "TrackAnimation.h"
#include "GameLoop.h"
#include "World.h"
#include <OgreRoot.h>
#include <OgreResourceGroupManager.h>
#include <OgreLight.h>
#include "ScriptObjectRepository.h"
%}

%exception rl::Animation::addAnimationListener {
	rl::ScriptObjectRepository* sor = rl::ScriptObjectRepository::getSingletonPtr();

	// Noch nicht enthalten
	if( !arg1->containsAnimationListener( arg2 ) )
	{	 
		// Originales einfügen
		$action
		
		// Skriptverknüpfung herstellen
		if( sor->getPtr( argv[0] ) == NULL )
			sor->insertPointerValuePair( arg2, argv[0] );		
		sor->incRefCount( argv[0] );
	}
}

%exception rl::Animation::removeAnimationListener {
	rl::ScriptObjectRepository* sor = rl::ScriptObjectRepository::getSingletonPtr();

	// Enthalten
	if( arg1->containsAnimationListener( arg2 ) )
	{	 
		// Originales entfernen
		$action							
		sor->decRefCount( argv[0] );
	}
}

%typemap(out) 
	rl::Actor*, rl::ActorControlledObject*
{
	rl::ScriptObjectRepository* sor = rl::ScriptObjectRepository::getSingletonPtr();
	VALUE val = sor->getScriptObject( $1 );
	
	// Es gab das SkriptObjekt noch nicht
	if( val == Qnil )
	{
		$result = SWIG_NewPointerObj((void *) $1, $1_descriptor, 0);
		sor->insertPointerValuePair( $1, $result );			
	}
	else	
		$result = val;
} 