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

#ifndef __UiSubsystem_H__
#define __UiSubsystem_H__

#include "UiPrerequisites.h"
#include "World.h"

#include <OgreSingleton.h>

#include "CharacterController.h"

namespace rl {

    class CommandMapper;
    class Creature;
    class GameActor;
	class GameObject;
	class InputManager;
	class Person;
	class RBCombat;
	class RTCombat;
	class WindowFactory;
	class WindowManager;

    class _RlUiExport UiSubsystem : public SceneChangeListener,
        protected Ogre::Singleton<UiSubsystem>
    {
    public:

        /** Default Constructor */
        UiSubsystem();
        /** Default Deconstructor */
        virtual ~UiSubsystem();

        /** Returns the Singleton */
        static UiSubsystem & getSingleton(void);
        static UiSubsystem * getSingletonPtr(void);

		void useDefaultAction(GameObject* obj, Creature* actor);
		void usePickedObjectDefaultActions();
		void toggleObjectPicking();

		Person* getActiveCharacter() const;
		void setActiveCharacter(Person* person);
        CharacterController* getCharacterController() const;
		CharacterController::ControllerType getCharacterControllerType() const;
		void setCharacterController(CharacterController::ControllerType type);

		void setCombatMode(bool inCombat);
		bool isInCombatMode();
		void startRBCombat(RBCombat* combat);
		void startRTCombat(RTCombat* combat);
	
		void update();

        /// from SceneChangeListener
        void onBeforeClearScene();

		static const char* CEGUI_ROOT;

    private:
        void initializeUiSubsystem( void );
        void runTest();
        
        CharacterController* mCharacterController;
        GameActor* mHero;
        Person* mCharacter;
		InputManager* mInputManager;
        bool mInCombat;
		WindowFactory* mWindowFactory;
		WindowManager* mWindowManager;
		CommandMapper* mCommandMapper;
    };
}

#endif
