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

/**
 * \file GameObjectInfoWindow.h
 *
 * Declaration of the GameObject window
 */

#ifndef __GameObjectInfoWindow_H__
#define __GameObjectInfoWindow_H__

#include "UiPrerequisites.h"
#include "AbstractWindow.h"

#include <vector>

namespace rl {

	class Action;
	class Creature;
	class GameObject;
    class Property;

    /**
     * The GameObject window class
     *
     * This window displays all properties of a game object by looping
     * through the property set of the given object.
     */
    class _RlUiExport GameObjectInfoWindow : public AbstractWindow
	{
	public:

        /**
         * Constructor of the GameObject window
         *
         * @param object The GameObject to be displayed
         */
		GameObjectInfoWindow(GameObject* object, Creature* character);
		
        /**
         * Destructor of the GameObject window
         */
        virtual ~GameObjectInfoWindow();

        /**
         * Initializes the window with the specified game object and
         * fills the window with the properties of the GameObject.
         */
		virtual void initialize();

	private:
		GameObject* mGameObject;
		Creature* mCharacter;

		CEGUI::Window* mIcon;
		CEGUI::MultiLineEditbox* mDescription;
		CEGUI::Window* mActionButtonsArea;
		int mNumActionButtons;

		void addActionButton(Action* action);
		bool handleActivateAction(Action* action) const;
	};
}

#endif
