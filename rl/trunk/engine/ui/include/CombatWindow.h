/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __CombatWindow_H__
#define __CombatWindow_H__

#include "UiPrerequisites.h"
#include "AbstractWindow.h"

namespace rl {

	/// Wraps all cegui gui aspects of the combat gui and is
	/// controlled by CombatGui class.
	class _RlUiExport CombatWindow : public AbstractWindow
	{
	public:
		typedef std::pair<int, int> SetSize;
		static const int ATTACK_BUTTON = 0;
		static const int PARRY_BUTTON = 1;

		CombatWindow();
		virtual ~CombatWindow();

		/// On-Screen size in pixels of a set of enemy action buttons (attack, parry, etc)
		SetSize getButtonSetSize();

		/// Add a set of action buttons for an enemy.
		int addEnemyButtonSet();
		/// Remove a set of action buttons for an enemy,
		/// identified by the handle as given by addEnemyButtonSet.
		void removeEnemyButtonSet(int handle);
		/// Position the buttons of an enabled button set.
		/// x and y are pixels
		void placeEnemyButtonSet(int handle, int x, int y);
		/// Enable or disable a button set.
		void enableEnemyButtonSet(int handle, bool enabled);
		void enableAllEnemyButtonSets(bool enabled);

		// Event handlers

		bool enemyButtonClicked(int handle, int buttonIndex);
		bool endTurnButtonClicked();

	private:
		typedef std::vector<CEGUI::Window*> ButtonVector;
		/// Key is the handle as returned by addEnemyButtonSet
		typedef std::map<int, ButtonVector> ButtonMap;

		ButtonMap mButtons;
		int mNextHandle;
		SetSize mSetSize;
		int mButtonPadding;
	};
}

#endif
