#ifndef __MainMenuWindow_H__
#define __MainMenuWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"

namespace rl {
	
	class GameObject;

	class _RlUiExport MainMenuWindow : public CeGuiWindow
	{
	public:
		MainMenuWindow(GameObject* actionHolder);

		bool handleChooseModules();
		bool handleGraphicOptions();
		bool handleStart();
		bool handleQuit();

		void setActiveModule(const CeGuiString& module);

	private:
		CeGuiString mActiveModule;
		GameObject* mActionHolder;
	};

	class _RlUiExport MainMenuChooseModulesWindow : public CeGuiWindow
	{
	public:
		MainMenuChooseModulesWindow(
			MainMenuWindow* parent, 
			const Ogre::StringVector& modules, 
			const CeGuiString& activeModule = CeGuiString(""));

		bool handleChooseOkay();
		bool handleChooseCancel();

	private:
		MainMenuWindow* mParent;
		CEGUI::Listbox* mModulesList;
	};



}

#endif
