#include "CeGuiWindow.h"

#include "InputManager.h"
#include "UiSubsystem.h"

using namespace std;
using namespace CEGUI;

namespace rl
{

int CeGuiWindow::sNumWindows = 0;

CeGuiWindow::CeGuiWindow(const char* xmlfile, bool withInput)
{
	mWindow = WindowManager::getSingleton().loadWindowLayout((utf8*)xmlfile/*, StringConverter::toString(sNumWindows)*/);
	sNumWindows++;

    mIsVisible = true;
    mWithInput = withInput;

	assert(mWindow != 0);
}

CeGuiWindow::~CeGuiWindow()
{
	WindowManager::getSingleton().destroyWindow(mWindow);
}

bool CeGuiWindow::isVisible()
{
    return mIsVisible;
}

void CeGuiWindow::setVisible(bool visible)
{
    if(mIsVisible != visible)
    {
        if (visible)
        {
            show();
        }
        else
        {
            hide();
        }
    }
}

void CeGuiWindow::show()
{
	if (!mIsVisible)
	{
		if (!beforeShow())
			return;

		if (isInputWindow())
			InputManager::getSingleton().registerCeguiWindow(this);
		mWindow->show();
        mIsVisible = true;
    }
}

void CeGuiWindow::hide()
{
	if (mIsVisible)
	{
		if (!beforeHide())
			return;
	
		mWindow->hide();
		if (isInputWindow())
			InputManager::getSingleton().unregisterCeguiWindow(this);
        mIsVisible = false;
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

MultiLineEditbox* CeGuiWindow::getMultiLineEditbox(const char* name)
{
	return reinterpret_cast<MultiLineEditbox*>(getWindow(name));
}

}

