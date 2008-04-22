/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#include "Action.h"
#include "ActionManager.h"
#include "Armor.h"
#include "Container.h"
#include "Creature.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "Date.h"
#include "DsaManager.h"
#include "Effect.h"
#include "EffectFactory.h"
#include "EigenschaftenStateSet.h"
#include "GameEventLog.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GlobalProperties.h"
#include "Inventory.h"
#include "Item.h"
#include "ObjectStateChangeEventSource.h"
#include "ObjectStateChangeEvent.h"
#include "ObjectStateChangeListener.h"
#include "Quest.h"
#include "QuestBook.h"
#include "QuestEvent.h"
#include "QuestListener.h"
#include "RulesConstants.h"
#include "RulesSubsystem.h"
#include "Selector.h"
#include "SonderfertigkeitenStateSet.h"
#include "StateSet.h"
#include "Talent.h"
#include "TalentStateSet.h"
#include "Tripel.h"
#include "WaitJob.h"
#include "Weapon.h"
%}
