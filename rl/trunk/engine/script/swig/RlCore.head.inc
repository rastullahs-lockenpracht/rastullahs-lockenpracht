/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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
#include "BSPWorld.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "ParticleSystemActor.h"
#include "GameActor.h"
#include "MeshActor.h"
#include "CoreSubsystem.h"
#include "ListenerActor.h"
#include "PhysicalThing.h"
#include "PhysicsManager.h"
#include "GameLoop.h"
#include "OgreRoot.h"
%}