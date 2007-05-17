/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

%{
//----------- Includes - Core -----------
#include "FixRubyHeaders.h"
#include "CorePrerequisites.h"
#include "ActorManager.h"
#include "Actor.h"
#include "MeshObject.h"
#include "MergeableMeshObject.h"
#include "SoundObject.h"
#include "SoundManager.h"
#include "ListenerObject.h"
#include "ListenerMovable.h"
#include "CameraObject.h"
#include "LightObject.h"
#include "ParticleSystemObject.h"
#include "CoreSubsystem.h"
#include "PhysicalThing.h"
#include "CoreDefines.h"
#include "PhysicsContactListener.h"
#include "DebugVisualsManager.h"
#include "GameEventManager.h"
#include "GameAreaEventSource.h"
#include "GameAreaListener.h"
#include "GameAreaEvent.h"
#include "GameAreaTypes.h"
#include "AnimationManager.h"
#include "AnimationListener.h"
#include "BaseAnimation.h"
#include "MeshAnimation.h"
#include "FadeAnimation.h"
#include "TrackAnimation.h"
#include "Trigger.h"
#include "GameLoop.h"
#include "World.h"
#include "ContentModule.h"
#include "ListenerObject.h"
#include "Sound.h"
#include "SoundEvents.h"
#include "JobScheduler.h"
#include "Job.h"
#include "JobListener.h"
#include "PlaySoundJob.h"
#include "PlayAnimationJob.h"
#include "Zone.h"
#include "ZoneManager.h"
//#include "LightFadeJob.h"
#undef message
#undef send
%}

