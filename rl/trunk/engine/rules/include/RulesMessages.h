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

#ifndef __Rl_RulesMessages_H__
#define __Rl_RulesMessages_H__

#include "RulesPrerequisites.h"

#include "MessageType.h"
#include "RulesConstants.h"
#include "CoreMessages.h"
#include "Effect.h"

namespace rl
{
	class Combatant;
	class GameObject;

	/// 0x03xxxxxx as mask for rules messages
    enum RulesMessageTypeIds
    {
        // Game event log //
        // Mask
        RLMSG_GAMEEVENTLOG_EVENTS = 0x03000100,
        // Events
        RLMSG_GAMEEVENTLOG_EVENT_ADDED = 0x03000101,

		// Combat
		// Mask
		RLMSG_COMBAT_EVENTS = 0x03000200,
		// Events
		RLMSG_COMBAT_OPPONENT_ENTERED = 0x03000201,
		RLMSG_COMBAT_OPPONENT_LEFT = 0x03000202,
		RLMSG_COMBAT_ENDED = 0x03000210,

		// Game object related events
        // Mask
        RLMSG_GAMEOBJECT_EVENTS = 0x03000300,
        // Events
        RLMSG_GAMEOBJECT_LIFESTATE_CHANGED = 0x03000301
    };

    /// Message sent right after a new game log event has been added.
	typedef MessageType<RLMSG_GAMEEVENTLOG_EVENT_ADDED, GameEventType, CeGuiString>
		MessageType_GameEventLog_EventAdded;

	typedef MessageType<RLMSG_COMBAT_OPPONENT_ENTERED, Combatant*>
		MessageType_CombatOpponentEntered;

	typedef MessageType<RLMSG_COMBAT_OPPONENT_LEFT, Combatant*>
		MessageType_CombatOpponentLeft;

    typedef MessageType<RLMSG_COMBAT_ENDED, bool>
		MessageType_CombatEnded;

    typedef MessageType<RLMSG_GAMEOBJECT_LIFESTATE_CHANGED, GameObject*, Effect::LifeState, Effect::LifeState>
		MessageType_GameObjectLifeStateChanged;
    
    typedef MessageType<RLMSG_SAVEGAME_GOS_LOADED> MessageType_GameObjectsLoaded;

}
#endif
