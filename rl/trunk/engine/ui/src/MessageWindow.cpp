#include "MessageWindow.h"

namespace rl {

MessageWindow::MessageWindow() :
	CeGuiWindow("messagewindow.xml", false)
{
	mTextField = getWindow("MessageWindow/Text");
	centerWindow();
	addToRoot(mWindow);
}

void MessageWindow::setText(const CeGuiString& message)
{
	mTextField->setText(message);
}

}
