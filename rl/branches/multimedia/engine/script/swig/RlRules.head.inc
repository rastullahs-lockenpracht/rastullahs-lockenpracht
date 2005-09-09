/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
//----------- Includes - Rules -----------
#include "FixRubyHeaders.h"
#include "RulesPrerequisites.h"
#include <CEGUIString.h>
#include "Date.h"
#include "Tripel.h"
#include "Talent.h"
#include "DsaManager.h"
#include "GameObject.h"
#include "Creature.h"
#include "Action.h"
#include "ActionManager.h"
#include "Item.h"
#include "Weapon.h"
#include "Person.h"
#include "RulesSubsystem.h"
#include "Container.h"
#include "ObjectStateChangeEventSource.h"
#include "ObjectStateChangeEvent.h"
#include "ObjectStateChangeListener.h"
#include "TimerEventSource.h"
#include "TimerEvent.h"
#include "TimerListener.h"
#include "Combat.h"
%}