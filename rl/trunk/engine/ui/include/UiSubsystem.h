#ifndef __UiSubsystem_H__
#define __UiSubsystem_H__

#include <OgreSingleton.h>
#include <string>

#include "UiPrerequisites.h"

namespace rl {

    class Interpreter;
    class ThirdPersonGameController;
    class GameActor;
	class GameObject;

    class _RlUiExport UiSubsystem : protected Singleton<UiSubsystem>
    {
    public:
        /** Default Constructor */
        UiSubsystem();
        /** Default Deconstructor */
        virtual ~UiSubsystem();

        /** Returns the Singleton */
        static UiSubsystem & getSingleton(void);
        static UiSubsystem * getSingletonPtr(void);


        /** Requests that the application exits */
        void requestExit();
        /** Gets the state of mRequestExit */
        bool isRequestingExit() const;

		/** Writes Text into the Console or other output media */
		void writeToConsole(std::string text);

		void showActionChoice(GameObject* obj);
		void testObj(rl::GameObject* obj){}
		void testStr(CeGuiString& obj){}

		static const char* CEGUI_ROOT;

    private:
        void initializeUiSubsystem( void );		void runTest();        ThirdPersonGameController* mGameController;        GameActor* mHero;        bool mRequestExit;    };

}

#endif
