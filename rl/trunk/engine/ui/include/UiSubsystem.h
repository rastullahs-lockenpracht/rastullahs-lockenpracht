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

#include <OgreSingleton.h>
#include <string>

namespace rl {

    class Interpreter;
    class CharacterController;
    class GameActor;
	class GameObject;
	class Person;
	class Combat;
	class Creature;
	class WindowFactory;
	class WindowManager;

    class _RlUiExport UiSubsystem : protected Ogre::Singleton<UiSubsystem>
    {
    public:

		enum ControllerType 
		{
			CTRL_FREEFLIGHT = 1,
			CTRL_MOVEMENT,
			CTRL_DIALOG,
			CTRL_COMBAT
		};

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

		Person* getActiveCharacter();
		void setActiveCharacter(Person* person);
        CharacterController* getCharacterController();
		void setCharacterController(ControllerType type);

		void setCombatMode(bool inCombat);
		bool isInCombatMode();
		void startCombat(Combat* combat);
	
		void update();

		static const char* CEGUI_ROOT;

    private:
        void initializeUiSubsystem( void );
        void runTest();
        
        CharacterController* mCharacterController;
        GameActor* mHero;
        Person* mCharacter;
        bool mInCombat;
		WindowFactory* mWindowFactory;
		WindowManager* mWindowManager;
    };
}

#endif
