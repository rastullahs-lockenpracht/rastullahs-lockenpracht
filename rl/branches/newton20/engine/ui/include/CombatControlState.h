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

#ifndef __CombatCharacterController_H__
#define __CombatCharacterController_H__

#include "UiPrerequisites.h"

#include "ControlState.h"
#include "Combatant.h"
#include "MessagePump.h"
#include "Selector.h"


namespace rl {

	class CameraObject;
    class Combat;
    class CombatGui;
    class CombatManager;

	class _RlUiExport CombatControlState : public ControlState, public Combatant
	{
	public:
		/**
		*  @throw NullPointerException if camera is NULL.
		*  @warning Ownership of combat is taken by the CombatControlState.
		*/
		CombatControlState(CommandMapper* cmdMapper, Actor* camera, Creature* character);
		virtual ~CombatControlState();

		// Control state overrides

        virtual void pause();
        virtual void resume();

		void run(Ogre::Real elapsedTime);

        // Combatant overrides

        virtual Ogre::String getCombatantTypeName() const;
        virtual void requestCombatantAction();

    private:
		enum State {REQUEST_USER_INPUT, ROUND_EXECUTION};

		MessagePump::ScopedConnection mCombatIoAttackOpponentConnection;
		MessagePump::ScopedConnection mCombatIoParryOpponentConnection;
		MessagePump::ScopedConnection mCombatIoEndTurnRequestedConnection;

	    MessagePump::ScopedConnection mEnemyLeftCombatConnection;
	    MessagePump::ScopedConnection mCombatEndConnection;

		Combatant* mAttackedOpponent;
		Combatant* mParriedOpponent;

        CombatManager* mCombatManager;
		CombatGui* mCombatGui;
        HalfSphereSelector mEnemySelector;

        CameraObject* mCamera;

		State mState;
        
        // Event handlers
		bool userRequestAttackOpponent(Combatant*);
		bool userRequestParryOpponent(Combatant*);
		bool userRequestEndTurn();

		bool enemyLeftCombat(Combatant*);
        bool combatEnded(bool);
	};
}
#endif
