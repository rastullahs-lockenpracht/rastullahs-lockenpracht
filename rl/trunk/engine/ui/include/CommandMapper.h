#ifndef __CommandMapper_H__
#define __CommandMapper_H__

#include "UiPrerequisites.h"

#include <OgreSingleton.h>
#include <OgreInput.h>
#include <map>

namespace rl {

	class CommandExecutor;

	enum MovementState
	{
		MOVE_NONE = 0,
		MOVE_LEFT = 1<<0,
		MOVE_RIGHT = 1<<1,
		MOVE_FORWARD = 1<<2,
		MOVE_BACKWARD = 1<<3,
		TURN_LEFT = 1<<4,
		TURN_RIGHT = 1<<5,
		MOVE_RUN = 1<<6,
		MOVE_SNEAK = 1<<7
	};


	class _RlUiExport CommandMapper : public Ogre::Singleton<CommandMapper>
	{
	public:

		CommandMapper();
		~CommandMapper();

		static CommandMapper& getSingleton();
		static CommandMapper* getSingletonPtr();

		void loadCommandMap(const char* mapfile);

		int getActiveMovement();
		bool isMovementActive(MovementState movmt);

		bool injectMouseClicked(int mouseButtonMask);
		bool injectKeyClicked(int keycode);		
		bool injectKeyDown(int keycode);
		bool injectKeyUp(int keycode);		

	private:
		// KeyCode -> (Rubyklasse, Name)
		typedef std::map<int, std::pair<CeGuiString, CeGuiString> > KeyCommandMap;
		typedef std::map<int, MovementState> MovementCommandMap;
		typedef std::map<int, std::pair<CeGuiString, CeGuiString> > MouseCommandMap;

		MovementCommandMap mMovementCommands;
		KeyCommandMap mKeyCommandsInBattle;
		KeyCommandMap mKeyCommandsOffBattle;
		MouseCommandMap mMouseCommands;

		int mActiveMovement;		
	};

}


#endif

