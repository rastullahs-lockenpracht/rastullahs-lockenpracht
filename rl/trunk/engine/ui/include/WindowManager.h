#ifndef __WindowManager_H__
#define __WindowManager_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>
#include <set>

namespace rl {

	class CeGuiWindow;

	class _RlUiExport WindowManager : public Ogre::Singleton<WindowManager>
	{
	public:
		WindowManager();

		static WindowManager& getSingleton();
		static WindowManager* getSingletonPtr();

		typedef std::set<CeGuiWindow*> WindowSet;

		void registerWindow(CeGuiWindow* window);
		bool destroyWindow(CeGuiWindow* window);

	private:
		WindowSet mWindows;
	};

}

#endif
