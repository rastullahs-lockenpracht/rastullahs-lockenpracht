#ifndef __UiSubsystem_H__
#define __UiSubsystem_H__

#include <OgreSingleton.h>

#include "UiPrerequisites.h"

namespace rl {

    class Interpreter;
    class ThirdPersonGameController;
    class GameActor;

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

    private:
        void initializeUiSubsystem( void );        ThirdPersonGameController* mGameController;        GameActor* mHero;        bool mRequestExit;    };

}

#endif
