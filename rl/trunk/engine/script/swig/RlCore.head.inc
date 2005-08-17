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
#include "TimerTask.h"
%}


// doWithActor( Actor* ) oder andere Director-Methoden Parameter
%typemap(directorin) SWIGTYPE*
{
	// Auf Director testen
	Swig::Director *resultdirector = 0;
	resultdirector = dynamic_cast<Swig::Director *>($1);
    if (resultdirector) {
        $input = resultdirector->swig_get_self();
    }
    else
	{
		rl::ScriptObjectRepository* sor = rl::ScriptObjectRepository::getSingletonPtr();
		VALUE val = sor->getScriptObject( $1 );
		
		// Es gab das SkriptObjekt noch nicht
		if( val == Qnil )
		{
			$input = SWIG_NewPointerObj((void *) $1, $1_descriptor, 0);
			sor->insertPointerValuePair( $1, $input );			
		}
		else	
			$input = val;
	}
} 

    
// Actor* getActor oder andere OUTPUT Parameter
%typemap(out) SWIGTYPE*
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


// Animation* getActor oder andere OUTPUT Parameter für DYNAMICs
%typemap(out) SWIGTYPE* DYNAMIC, SWIGTYPE& DYNAMIC
{
	rl::ScriptObjectRepository* sor = rl::ScriptObjectRepository::getSingletonPtr();
	VALUE val = sor->getScriptObject( $1 );
	
	// Es gab das SkriptObjekt noch nicht
	if( val == Qnil )
	{
		// Dynamic Cast ausführen
		swig_type_info *ty = SWIG_TypeDynamicCast($1_descriptor, (void **) &$1);
		$result = SWIG_NewPointerObj((void *) $1, ty, 0);
		sor->insertPointerValuePair( $1, $result );			
	}
	else	
		$result = val;
} 

// doWithAnimation( Animation* ) oder andere Director-Methoden Parameter für DYNAMICs
%typemap(directorin) SWIGTYPE* DYNAMIC, SWIGTYPE& DYNAMIC
{
	// Auf Director testen
	Swig::Director *resultdirector = 0;
	resultdirector = dynamic_cast<Swig::Director *>($1);
    if (resultdirector) {
        $input = resultdirector->swig_get_self();
    }
    else
	{
		rl::ScriptObjectRepository* sor = rl::ScriptObjectRepository::getSingletonPtr();
		VALUE val = sor->getScriptObject( $1 );
		
		// Es gab das SkriptObjekt noch nicht
		if( val == Qnil )
		{
			// Dynamic Cast ausführen
			swig_type_info *ty = SWIG_TypeDynamicCast($1_descriptor, (void **) &$1);
			$input = SWIG_NewPointerObj((void *) $1, ty, 0);
			sor->insertPointerValuePair( $1, $input );			
		}
		else	
			$input = val;
	}
} 