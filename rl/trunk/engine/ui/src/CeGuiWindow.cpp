#include "CeGuiWindow.h"

using namespace std;
using namespace CEGUI;

namespace rl
{

CeGuiWindow::CeGuiWindow(const char* xmlfile, bool withInput)
{
	mWindow = 
		WindowManager::getSingleton().loadWindowLayout((utf8*)xmlfile);
	mWithInput = withInput;
}

CeGuiWindow::~CeGuiWindow()
{
	WindowManager::getSingleton().destroyWindow(mWindow);
}

void CeGuiWindow::show()
{
	if (mState == CS_OPEN)
	{
		if (!beforeOpen())
			return;

		mState = CS_OPENING;
		if (isWithInput())
			InputManager::getSingleton().registerCeguiWindow();
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
		if (isWithInput())
			InputManager::getSingleton().unregisterCeguiWindow();
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

}