#include <boost/bind.hpp>

#include "Action.h"
#include "ActionChoiceWindow.h"
#include "ActionManager.h"
#include "GameObject.h"
#include "Creature.h"
#include "UiSubsystem.h"

using namespace CEGUI;
using namespace std;

namespace rl {

	ActionChoiceWindow::ActionChoiceWindow()
		: CeGuiWindow("actionchoicewindow.xml", true)
	{
		mHint = getStaticText("ActionChoiceWindow/Hint");
		addToRoot(mWindow);
	}
	
	ActionChoiceWindow::~ActionChoiceWindow()
	{
		for (unsigned int i = 0; i<mButtons.size(); i++)
		{
			mWindow->removeChildWindow(mButtons[i]);
			WindowManager::getSingleton().destroyWindow(mButtons[i]);
		}
		mButtons.clear();
	}
	
	void ActionChoiceWindow::showActionsOfObject(GameObject* object)
	{
		mObject = object;
		
		for (unsigned int i = 0; i<mButtons.size(); i++)
		{
			mWindow->removeChildWindow(mButtons[i]);
			WindowManager::getSingleton().destroyWindow(mButtons[i]);
		}
		mButtons.clear();
	
		StringVector actions = object->getValidActions();
		Point center(300, 200);
		static int RADIUS = 40;
	
		for (	StringVector::iterator iter = actions.begin(); 
				iter != actions.end(); iter++)
		{
			string actionName = *iter;
			Action* action = 
				ActionManager::getSingleton().getDefaultAction(actionName);
			PushButton* actionButton = reinterpret_cast<PushButton*>(
				//WindowManager::getSingleton().loadWindowLayout(
				//	"buttons/"+actionName+".xml"));
				WindowManager::getSingleton().createWindow(
					(utf8*)"Taharez Button", 
					CEGUI::String("ActionChoiceWindow/Buttons/"+actionName)));
			actionButton->setText(CEGUI::String(action->getDescription()));
			Point pos = getPositionOnRadialMenu(
					center, 0, 0, RADIUS, mButtons.size(), actions.size());
			actionButton->setPosition(Absolute, pos);
			actionButton->setSize(Absolute, Size(40, 40));
			actionButton->subscribeEvent(
				PushButton::Clicked, 
				boost::bind(
					&ActionChoiceWindow::handleActionChosen, 
					this, actionName));
			actionButton->subscribeEvent(
				PushButton::MouseEntersEvent,
				boost::bind(&ActionChoiceWindow::handleShowHint, this, _1));
			actionButton->subscribeEvent(
				PushButton::MouseLeavesEvent,
				boost::bind(&ActionChoiceWindow::handleRemoveHint, this));
			mButtons.push_back(actionButton);
			mWindow->addChildWindow(actionButton);
		}
	}
	
	void ActionChoiceWindow::handleActionChosen(string& action)
	{
		//TODO: Auswahl des Ziels/der Ziele
		//TODO: Ausf�hrung der Action, hier im Dialog oder doch woanders?
	}
	
	void ActionChoiceWindow::handleShowHint(const EventArgs& evt)
	{
		mHint->setText((utf8*)"");
	}
	
	void ActionChoiceWindow::handleRemoveHint()
	{
		mHint->setText((utf8*)"");
	}
	
	void ActionChoiceWindow::showTalentsOfPerson(Creature* creature)
	{
		
	}
	
	Point ActionChoiceWindow::getPositionOnRadialMenu(
		Point center, float minAngle, float maxAngle,
		float radius, int elemNum, int elemCount)
	{
		static const float PI = 3.1415926;
		
		if (maxAngle <= minAngle)
			maxAngle += 360;
			
		float angle = 
			minAngle + 
			(float)elemNum/(float)elemCount * (maxAngle - minAngle);
		if (angle > 360)
			angle -= 360;
		
		float relX = radius * sin(PI * angle/180);
		float relY = radius * cos(PI * angle/180);
		
		return center + Point(relX, relY);
	}

}
