#include "CeGuiWindow.h"

#include "InputManager.h"
#include "UiSubsystem.h"

using namespace std;
using namespace CEGUI;

namespace rl
{

CeGuiWindow::CeGuiWindow(const char* xmlfile, bool withInput)
{
	mWindow = 
		WindowManager::getSingleton().loadWindowLayout((utf8*)xmlfile);
	assert(mWindow != 0);

	mWithInput = withInput;
}

CeGuiWindow::~CeGuiWindow()
{
	WindowManager::getSingleton().destroyWindow(mWindow);
}

void CeGuiWindow::show()
{
	if (mState == CS_CLOSED)
	{
		if (!beforeShow())
			return;

		mState = CS_OPENING;
		if (isInputWindow())
			InputManager::getSingleton().registerCeguiWindow(this);
		mWindow->show();
		mState = CS_OPEN;
	}
}

void CeGuiWindow::hide()
{
	if (mState == CS_OPEN)
	{
		if (!beforeHide())
			return;
	
		mState = CS_CLOSING;
		mWindow->hide();
		if (isInputWindow())
			InputManager::getSingleton().unregisterCeguiWindow(this);
		mState = CS_CLOSED;
	}
}

bool CeGuiWindow::isInputWindow()
{
	return mWithInput;
}

bool CeGuiWindow::beforeHide()
{
	return true;
}

bool CeGuiWindow::beforeShow()
{
	return true;
}

void CeGuiWindow::addToRoot(Window* window)
{
	getWindow(UiSubsystem::CEGUI_ROOT)->addChildWindow(window);
}

Window* CeGuiWindow::getWindow(const char* name)
{
	return WindowManager::getSingleton().getWindow((utf8*)name);
}

Editbox* CeGuiWindow::getEditbox(const char* name)
{
	return reinterpret_cast<Editbox*>(getWindow(name));
}

Listbox* CeGuiWindow::getListbox(const char* name)
{
	return reinterpret_cast<Listbox*>(getWindow(name));
}

StaticText* CeGuiWindow::getStaticText(const char* name)
{
	return reinterpret_cast<StaticText*>(getWindow(name));
}

StaticImage* CeGuiWindow::getStaticImage(const char* name)
{
	return reinterpret_cast<StaticImage*>(getWindow(name));
}

MultiColumnList* CeGuiWindow::getMultiColumnList(const char* name)
{
	return reinterpret_cast<MultiColumnList*>(getWindow(name));
}

}

