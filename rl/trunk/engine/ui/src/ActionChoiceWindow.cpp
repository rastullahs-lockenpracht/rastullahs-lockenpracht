#include <boost/bind.hpp>

#include "ActionChoiceWindow.h"
#include "ActionManager.h"
#include "GameObject.h"

using namespace CEGUI;
using namespace std;

namespace rl {

ActionChoiceWindow::ActionChoiceWindow()
	: CeGuiWindow("actionchoicewindow.xml", true)
{
	addToRoot(mWindow);
}

ActionChoiceWindow::~ActionChoiceWindow()
{
	//TODO: Buttons aufräumen
}

void ActionChoiceWindow::setActionObject(GameObject* object)
{
	mObject = object;
	
	for (unsigned int i = 0; i<mButtons.size(); i++)
	{
		mWindow->removeChildWindow(mButtons[i]);
		WindowManager::getSingleton().destroyWindow(mButtons[i]);
	}
	mButtons.clear();

	StringVector actions = object->getValidActions();

	for (StringVector::iterator iter = actions.begin(); iter != actions.end(); iter++)
	{
		string actionName = *iter;
		Action* action = ActionManager::getSingleton().getDefaultAction(actionName);
		PushButton* actionButton = reinterpret_cast<PushButton*>(WindowManager::getSingleton().loadWindowLayout("buttons/"+actionName+".xml"));
		int x = 40 * mButtons.size();
		int y = 200;
		actionButton->setPosition(Absolute, Point(x, y));
		actionButton->subscribeEvent(
			PushButton::Clicked, 
			boost::bind(&ActionChoiceWindow::handleActionChosen, this, actionName));
		mButtons.push_back(actionButton);
		mWindow->addChildWindow(actionButton);
	}
}

void ActionChoiceWindow::handleActionChosen(string& action)
{
	//TODO: Auswahl des Ziels/der Ziele
	//TODO: Ausführung der Action, hier im Dialog oder doch woanders?
}

}
